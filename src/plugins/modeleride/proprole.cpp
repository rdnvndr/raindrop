#include "proprole.h"

#include <QMdiSubWindow>

#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropRole::PropRole(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(roleWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(roleWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(roleWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(roleWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));

    connect(toolButtonAddRole,  SIGNAL(clicked()), roleWidget, SLOT(add()));
    connect(toolButtonDelRole,  SIGNAL(clicked()), roleWidget, SLOT(remove()));
    connect(toolButtonEditRole, SIGNAL(clicked()), roleWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), roleWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), roleWidget, SLOT(submit()));
}

PropRole::~PropRole()
{

}

void PropRole::setModel(TreeXmlHashModel *model)
{
    roleWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropRole::dataId(const QModelIndex &index)
{
    return modelData(DBROLEXML::ROLE, DBROLEXML::ID,index).toString();
}

void PropRole::setCurrent(const QModelIndex &index)
{
     roleWidget->setCurrent(index);
}

void PropRole::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString roleName = modelData(DBROLEXML::ROLE, DBROLEXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropRole::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(roleName);
}

void PropRole::edit(bool flag)
{
    if (roleWidget->isEmpty()){
        toolButtonAddRole->setDisabled(true);
        flag = true;
    } else
        toolButtonAddRole->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditRole->setDisabled(flag);
}

}}
