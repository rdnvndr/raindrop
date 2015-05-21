#include "propentity.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropEntity::PropEntity(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(entityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            unitWidget,SLOT(setRootIndex(QModelIndex)));
    connect(entityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(entityWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(entityWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
    connect(unitWidget,SIGNAL(proxyIndexChanged(QModelIndex)),
            entityWidget, SLOT(setUnitRootIndex(QModelIndex)));

    connect(entityWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(entityWidget, SIGNAL(edited(bool)), unitWidget, SLOT(edit(bool)));

    connect(toolButtonAddEntity,  SIGNAL(clicked()), entityWidget, SLOT(add()));
    connect(toolButtonDelEntity,  SIGNAL(clicked()), entityWidget, SLOT(remove()));
    connect(toolButtonEditEntity, SIGNAL(clicked()), entityWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), unitWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), entityWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), unitWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), entityWidget, SLOT(submit()));

}

PropEntity::~PropEntity()
{

}

void PropEntity::setModel(TreeXmlHashModel *model)
{
    entityWidget->setModel(model);
    unitWidget->setModel(model);

    entityWidget->setUnitModel(unitWidget->proxyModel());
    entityWidget->setUnitColumn(
                model->columnDisplayedAttr(
                    DBUNITXML::UNIT,
                    DBUNITXML::NAME
                    )
                );
    AbstractPropEditor::setModel(model);
}

void PropEntity::setCurrent(const QModelIndex &index)
{
    entityWidget->setCurrent(index);
}

void PropEntity::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString entityName = modelData(DBENTITYXML::ENTITY, DBENTITYXML::NAME,index).toString();
    QString id = modelData(DBENTITYXML::ENTITY, DBENTITYXML::ID,index).toString();

    this->setObjectName("PropEntity::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(entityName);
}

void PropEntity::edit(bool flag)
{
    if (entityWidget->isEmpty()){
        toolButtonAddEntity->setDisabled(true);
        flag = true;
    } else
        toolButtonAddEntity->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditEntity->setDisabled(flag);
}

}}
