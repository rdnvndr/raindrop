#include "msrentitywidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>
#include <QDebug>

MsrEntityWidget::MsrEntityWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    connect(comboBoxBasicUnit, SIGNAL(activated(int)),
            this, SLOT(changeUnit(int)));

    m_oldIndex = -1;
}

MsrEntityWidget::~MsrEntityWidget()
{
    delete m_mapper;
}

void MsrEntityWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditEntityName,
                         model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                   DBENTITYXML::NAME));
    m_mapper->addMapping(lineEditEntityDesc,
                         model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                   DBENTITYXML::DESCRIPTION));
    m_mapper->addMapping(lineEditDimensionSymbol,
                         model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                    DBENTITYXML::DIMENSIONSYMBOL));
    m_mapper->addMapping(comboBoxBasicUnit,
                         model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                    DBENTITYXML::BASICUNIT));

}

bool MsrEntityWidget::isRemove(const QModelIndex &srcIndex)
{
    bool success = true;
    QString msg;

    QString tag = srcIndex.data(Qt::UserRole).toString();
    QString fieldId = m_model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     m_model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXmlHashModel::TagWithAttr tagWithAttr,
             m_model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = m_model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid()) {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QString parentName;
                QString name;
                if (linkIndex.data(Qt::UserRole) == DBCLASSXML::CLASS) {
                    name = tr("класс ")
                            + linkIndex.sibling(linkIndex.row(),
                                                m_model->columnDisplayedAttr(
                                                    DBCLASSXML::CLASS,
                                                    DBCLASSXML::NAME)
                                                ).data().toString();
                } else {
                    if (linkParent.data(Qt::UserRole) == DBCOMPXML::COMP)
                        parentName = tr(" принадлежащий составу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    m_model->columnDisplayedAttr(
                                        DBCOMPXML::COMP,
                                        DBCOMPXML::NAME)
                                    ).data().toString();
                    else
                        parentName = tr(" принадлежащий классу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    m_model->columnDisplayedAttr(
                                        DBCLASSXML::CLASS,
                                        DBCLASSXML::NAME)
                                    ).data().toString();

                    if  (linkIndex.data(Qt::UserRole) == DBCOMPXML::COMP)
                        name = tr("состав ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    m_model->columnDisplayedAttr(
                                                        DBCOMPXML::COMP,
                                                        DBCOMPXML::NAME)
                                                    ).data().toString();
                    else
                        name = tr("атрибут ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    m_model->columnDisplayedAttr(
                                                        DBATTRXML::ATTR,
                                                        DBATTRXML::NAME)
                                                    ).data().toString();

                }
                msg += QString(tr("Необходимо удалить %1%2.\n\n")).
                        arg(name).arg(parentName);
                if (success)
                    success = false;
            }
            number++;
            linkIndex = m_model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

bool MsrEntityWidget::isEmpty()
{
    return lineEditEntityName->text().isEmpty();
}

void MsrEntityWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    m_model->setInsTagName(DBENTITYXML::ENTITY);
    if (m_model->insertRow(0,srcIndex)) {
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void MsrEntityWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void MsrEntityWidget::removeEmpty()
{
    if (lineEditEntityName->text().isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrent(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}

void MsrEntityWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void MsrEntityWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxEntity->setEnabled(flag);

    emit edited(flag);
}

void MsrEntityWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBENTITYXML::ENTITY,
                                                     DBENTITYXML::NAME,
                                                     lineEditEntityName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Сущность ЕИ с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void MsrEntityWidget::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    removeEmpty();
    edit(false);
}

void MsrEntityWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    if (index == m_mapper->rootIndex()){
        if (m_oldIndex > end)
            m_oldIndex = m_oldIndex - end-start-1;
        else if (m_oldIndex >= start && m_oldIndex <= end){
            m_oldIndex = -1;
        }
    }

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1 && m_oldIndex <0)
        emit dataRemoved(QModelIndex());
}

void MsrEntityWidget::setUnitModel(QAbstractItemModel *model)
{
    comboBoxBasicUnit->setModel(model);
}

void MsrEntityWidget::setUnitRootIndex(const QModelIndex &index)
{
    comboBoxBasicUnit->setRootModelIndex(index);
}

void MsrEntityWidget::setUnitColumn(int column)
{
    comboBoxBasicUnit->setModelColumn(column);
}

void MsrEntityWidget::changeUnit(int current)
{
    Q_UNUSED(current)

    // Для перевода ЕИ необходимо разделить на текущий koeff и отнять delta
    // т.е. x/koeff - delta

    QModelIndex index  = comboBoxBasicUnit->view()->currentIndex();
    QModelIndex parent = comboBoxBasicUnit->rootModelIndex();
    QAbstractItemModel *model = comboBoxBasicUnit->model();

    int count  = parent.model()->rowCount(parent);
    int columnCoeff = m_model->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::COEFF);
    int columnDelta = m_model->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::DELTA);

    float coeff = index.sibling(index.row(),columnCoeff).data().toFloat();
    float delta = index.sibling(index.row(),columnDelta).data().toFloat();
    coeff = (coeff==0)? 1: coeff;

    for (int row = 0; row < count; row++) {
        index = parent.child(row, columnCoeff);
        model->setData(index,index.data().toFloat()/coeff);
        index = parent.child(row, columnDelta);
        model->setData(index,index.data().toFloat()-delta);
    }
}

QVariant MsrEntityWidget::modelData(const QString &tag, const QString &attr,
                                    const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}



