#include "msrentitywidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>

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

    connect(m_model,SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            this,SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));

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
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBUNITXML::UNIT;
    if (model->hasChildren(srcIndex,tags)) {
        msg += tr("Необходимо удалить ЕИ.\n\n");
        success = false;
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
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBENTITYXML::ENTITY);
    if (srcCurrentIndex.isValid()) {
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

            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

            if (!isRemove(srcIndex))
                return;

            QModelIndex parent = srcIndex.parent();
            setCurrent(parent);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            setCurrent(parent.child(m_oldIndex,0));
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
    edit(false);
    emit dataChanged(srcIndex);
    removeEmpty();
}

void MsrEntityWidget::revert()
{
    m_mapper->revert();
    edit(false);
    removeEmpty();   
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

void MsrEntityWidget::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    for (int row = end; row >= start; row--) {
        if (comboBoxBasicUnit->currentIndex() == row) {
            comboBoxBasicUnit->setCurrentIndex(-1);
            return;
        }
    }
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
        model->setData(index,QString("%1").arg(index.data().toFloat()/coeff));
        index = parent.child(row, columnDelta);
        model->setData(index,QString("%1").arg(index.data().toFloat()-delta));
    }
}

QVariant MsrEntityWidget::modelData(const QString &tag, const QString &attr,
                                    const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}



