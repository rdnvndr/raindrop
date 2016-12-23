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

    connect(roleWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropRole::setTabName);
    connect(roleWidget, &AbstractEditorWidget::dataChanged,
            this, &PropRole::setTabName);
    connect(roleWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);

    connect(roleWidget, &RoleWidget::edited, this,
            static_cast<void (AbstractPropEditor::*)(bool flag)>(&AbstractPropEditor::edit));

    connect(toolButtonAddRole, &QToolButton::clicked,
            roleWidget, &RoleWidget::add);
    connect(toolButtonDelRole, &QToolButton::clicked,
            roleWidget, &RoleWidget::remove);
    connect(toolButtonEditRole, &QToolButton::clicked, roleWidget,
            static_cast<void (AbstractEditorWidget::*)()>(&AbstractEditorWidget::edit));

    connect(pushButtonPropCancel, &QPushButton::clicked, roleWidget, &RoleWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked, roleWidget, &RoleWidget::submit);
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
    } else
        toolButtonAddRole->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditRole->setDisabled(flag);
}

}}
