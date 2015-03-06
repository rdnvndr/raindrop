#include "propentitygroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

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

    m_oldIndex = QModelIndex();
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
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBENTITYXML::ENTITY;
    if (model->hasChildren(srcIndex,tags)) {
        msg += tr("Необходимо удалить сущности ЕИ.\n\n");
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

bool PropEntityGroup::isEmpty()
{
    return lineEditEntityGroupName->text().isEmpty();
}

void PropEntityGroup::add()
{
    m_oldIndex =  m_model->index(m_mapper->currentIndex(),
                                 0,m_mapper->rootIndex());
    QModelIndex srcIndex = m_oldIndex.parent();
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBENTITYGROUPXML::ENTITYGROUP);
    if (srcCurrentIndex.isValid()) {
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void PropEntityGroup::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

bool PropEntityGroup::removeEmpty()
{
    if (isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();        
        }
        return true;
    }
    return false;
}

void PropEntityGroup::setCurrent(const QModelIndex &index)
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

    this->setObjectName("PropEntityGroup::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(entityGroupName);
}

void PropEntityGroup::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

void PropEntityGroup::edit(bool flag)
{

    if (groupBoxEntityGroup->isEnabled()==flag)
        return;

    if (isEmpty()) {
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    groupBoxEntityGroup->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);

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
    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void PropEntityGroup::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    edit(false);
    removeEmpty();
}

void PropEntityGroup::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED (start)
    Q_UNUSED (end)
    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

QVariant PropEntityGroup::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

}}
