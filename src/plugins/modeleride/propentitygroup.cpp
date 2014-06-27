#include "propentitygroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include "dbxmlstruct.h"
#include "xmldelegate.h"

PropEntityGroup::PropEntityGroup(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(this,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(this,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(this,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));


    connect(toolButtonAdd,  SIGNAL(clicked()), this, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), this, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), this, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), this, SLOT(submit()));

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_oldIndex = -1;
}

PropEntityGroup::~PropEntityGroup()
{
    delete m_mapper;
}

void PropEntityGroup::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditEntityGroupName,
                         model->columnDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                                    DBENTITYXML::NAME));
    m_mapper->addMapping(lineEditEntityGroupDesc,
                         model->columnDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                                   DBENTITYGROUPXML::DESCRIPTION));
}

TreeXmlHashModel *PropEntityGroup::model()
{
    return m_model;
}

bool PropEntityGroup::isRemove(const QModelIndex &srcIndex)
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

bool PropEntityGroup::isEmpty()
{
    return lineEditEntityGroupName->text().isEmpty();
}

void PropEntityGroup::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    m_model->setInsTagName(DBENTITYGROUPXML::ENTITYGROUP);
    if (m_model->insertRow(0,srcIndex)) {
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrentEntityGroup(srcCurrentIndex);
        edit(true);
    }
}

void PropEntityGroup::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrentEntityGroup(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void PropEntityGroup::removeEmpty()
{
    if (isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrentEntityGroup(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}

void PropEntityGroup::setCurrentEntityGroup(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void PropEntityGroup::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString entityGroupName = modelData(
                DBENTITYGROUPXML::ENTITYGROUP,
                DBENTITYGROUPXML::NAME,index).toString();
    QString id = modelData(DBENTITYGROUPXML::ENTITYGROUP,
                           DBENTITYGROUPXML::ID,
                           index).toString();

    this->setObjectName("PropEntity::" + id);
    subWindow->setWindowTitle(tr("Сущность ЕИ: ")+entityGroupName);
}

void PropEntityGroup::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

void PropEntityGroup::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxEntityGroup->setEnabled(flag);

    emit edited(flag);
}

void PropEntityGroup::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(
                DBENTITYGROUPXML::ENTITYGROUP,
                DBENTITYGROUPXML::NAME,
                lineEditEntityGroupName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа сущность ЕИ с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void PropEntityGroup::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrentEntityGroup(srcIndex);
    removeEmpty();
    edit(false);
}

void PropEntityGroup::rowsRemoved(const QModelIndex &index, int start, int end)
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

QVariant PropEntityGroup::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}
