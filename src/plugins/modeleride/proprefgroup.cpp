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

    connect(refGroupWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refGroupWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refGroupWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

PropRefGroup::~PropRefGroup()
{

}

void PropRefGroup::setModel(TreeXmlHashModel *model)
{
    refGroupWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

void PropRefGroup::setCurrent(const QModelIndex &index)
{
    refGroupWidget->setCurrent(index);
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
    refGroupWidget->edit(flag);
}

}}
