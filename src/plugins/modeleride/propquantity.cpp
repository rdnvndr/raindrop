#include "propquantity.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropQuantity::PropQuantity(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(quantityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            unitWidget,SLOT(setRootIndex(QModelIndex)));
    connect(quantityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(quantityWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(quantityWidget,SIGNAL(dataAboutToBeRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
    connect(unitWidget,SIGNAL(proxyIndexChanged(QModelIndex)),
            quantityWidget, SLOT(setUnitRootIndex(QModelIndex)));

    connect(quantityWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(quantityWidget, SIGNAL(edited(bool)), unitWidget, SLOT(edit(bool)));

    connect(toolButtonAddQuantity,  SIGNAL(clicked()), quantityWidget, SLOT(add()));
    connect(toolButtonDelQuantity,  SIGNAL(clicked()), quantityWidget, SLOT(remove()));
    connect(toolButtonEditQuantity, SIGNAL(clicked()), quantityWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), unitWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), quantityWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), unitWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), quantityWidget, SLOT(submit()));

}

PropQuantity::~PropQuantity()
{

}

void PropQuantity::setModel(TreeXmlHashModel *model)
{
    quantityWidget->setModel(model);
    unitWidget->setModel(model);

    quantityWidget->setUnitModel(unitWidget->proxyModel());
    quantityWidget->setUnitColumn(
                model->columnDisplayedAttr(
                    DBUNITXML::UNIT,
                    DBUNITXML::NAME
                    )
                );
    AbstractPropEditor::setModel(model);
}

QString PropQuantity::dataId(const QModelIndex &index)
{
    return modelData(DBQUANTITYXML::QUANTITY, DBQUANTITYXML::ID,index).toString();
}

void PropQuantity::setCurrent(const QModelIndex &index)
{
    quantityWidget->setCurrent(index);
}

void PropQuantity::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString quantityName = modelData(DBQUANTITYXML::QUANTITY, DBQUANTITYXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropQuantity::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(quantityName);
}

void PropQuantity::edit(bool flag)
{
    if (quantityWidget->isEmpty()){
        toolButtonAddQuantity->setDisabled(true);
    } else
        toolButtonAddQuantity->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditQuantity->setDisabled(flag);
}

}}
