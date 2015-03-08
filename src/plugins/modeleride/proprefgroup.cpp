#include "proprefgroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropRefGroup::PropRefGroup(QWidget *parent) :
    AbstractPropEditor(parent)
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

PropRefGroup::~PropRefGroup()
{
    delete m_mapper;
}

void PropRefGroup::setModel(TreeXmlHashModel *model)
{
    AbstractPropEditor::setModel(model);

    connect(model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(model);

    m_mapper->addMapping(lineEditRefGroupName,
                         model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                    DBREFGROUPXML::NAME));
    m_mapper->addMapping(lineEditRefGroupDesc,
                         model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                    DBREFGROUPXML::DESCRIPTION));
}

bool PropRefGroup::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBREFXML::REF;
    if (model->hasChildren(srcIndex,tags)) {
        msg += tr("Необходимо удалить справочник.\n\n");
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

bool PropRefGroup::isEmpty()
{
    return lineEditRefGroupName->text().isEmpty();
}

void PropRefGroup::add()
{
    m_oldIndex =  model()->index(m_mapper->currentIndex(),
                                 0,m_mapper->rootIndex());
    QModelIndex srcIndex = m_oldIndex.parent();
    QModelIndex srcCurrentIndex =
            model()->insertLastRows(0,1,srcIndex,DBREFGROUPXML::REFGROUP);
    if (srcCurrentIndex.isValid()) {
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void PropRefGroup::remove()
{
    QModelIndex srcIndex = model()->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    model()->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

bool PropRefGroup::removeEmpty()
{
    if (isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = model()->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            model()->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();    
        }
        return true;
    }
    return false;
}

void PropRefGroup::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void PropRefGroup::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString refGroupName = modelData(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,index).toString();
    QString id = modelData(DBREFGROUPXML::REFGROUP,
                           DBREFGROUPXML::ID,
                           index).toString();

    this->setObjectName("PropRefGroup::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(refGroupName);
}

void PropRefGroup::edit(bool flag)
{

    if (groupBoxRefGroup->isEnabled()==flag)
        return;

    if (isEmpty()) {
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    groupBoxRefGroup->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);

    emit edited(flag);
}

void PropRefGroup::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,
                lineEditRefGroupName->text());
    QModelIndex srcIndex = model()->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа справочника с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void PropRefGroup::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = model()->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    edit(false);
    removeEmpty();
}

void PropRefGroup::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED (start)
    Q_UNUSED (end)
    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

}}
