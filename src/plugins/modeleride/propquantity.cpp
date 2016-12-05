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

    connect(quantityWidget, &QuantityWidget::currentIndexChanged,
            unitWidget, &UnitWidget::setRootIndex);
    connect(quantityWidget, &QuantityWidget::currentIndexChanged,
            this, &PropQuantity::setTabName);
    connect(quantityWidget, &QuantityWidget::dataChanged,
            this, &PropQuantity::setTabName);
    connect(quantityWidget, &QuantityWidget::dataAboutToBeRemoved,
            this, &PropQuantity::closeTab);
    connect(unitWidget, &UnitWidget::proxyIndexChanged,
            quantityWidget, &QuantityWidget::setUnitRootIndex);

    connect(quantityWidget, &QuantityWidget::edited, this,
            static_cast<void (AbstractPropEditor::*)()>(&AbstractPropEditor::edit));
    connect(quantityWidget, &QuantityWidget::edited, unitWidget,
            static_cast<void (AbstractModifyWidget::*)()>(&AbstractModifyWidget::edit));

    connect(toolButtonAddQuantity, &QToolButton::clicked,
            quantityWidget, &QuantityWidget::add);
    connect(toolButtonDelQuantity, &QToolButton::clicked,
            quantityWidget, &QuantityWidget::remove);
    connect(toolButtonEditQuantity, &QToolButton::clicked, quantityWidget,
            static_cast<void (AbstractEditorWidget::*)()>(&AbstractEditorWidget::edit));

    connect(pushButtonPropCancel, &QPushButton::clicked,
            unitWidget, &UnitWidget::revert);
    connect(pushButtonPropCancel, &QPushButton::clicked,
            quantityWidget, &QuantityWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked,
            unitWidget, &UnitWidget::submit);
    connect(pushButtonPropSave, &QPushButton::clicked,
            quantityWidget, &QuantityWidget::submit);

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
