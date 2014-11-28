#include "proprefgroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include "dbxmlstruct.h"
#include "xmldelegate.h"

PropRefGroup::PropRefGroup(QWidget *parent) :
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

PropRefGroup::~PropRefGroup()
{
    delete m_mapper;
}

void PropRefGroup::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditRefGroupName,
                         model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                    DBREFGROUPXML::NAME));
    m_mapper->addMapping(lineEditRefGroupDesc,
                         model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                    DBREFGROUPXML::DESCRIPTION));
}

TreeXmlHashModel *PropRefGroup::model()
{
    return m_model;
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
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBREFGROUPXML::REFGROUP);
    if (srcCurrentIndex.isValid()) {
        setCurrentRefGroup(srcCurrentIndex);
        edit(true);
    }
}

void PropRefGroup::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrentRefGroup(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void PropRefGroup::removeEmpty()
{
    if (isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrentRefGroup(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}

void PropRefGroup::setCurrentRefGroup(const QModelIndex &index)
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
    subWindow->setWindowTitle(tr("Группа справочника: ")+refGroupName);
}

void PropRefGroup::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
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
    QModelIndex existIndex = m_model->indexHashAttr(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,
                lineEditRefGroupName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа справочника с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void PropRefGroup::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrentRefGroup(srcIndex);
    removeEmpty();
    edit(false);
}

void PropRefGroup::rowsRemoved(const QModelIndex &index, int start, int end)
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

QVariant PropRefGroup::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}
