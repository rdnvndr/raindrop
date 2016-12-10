#include "propquantitygroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropQuantityGroup::PropQuantityGroup(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(quantityGroupWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropQuantityGroup::setTabName);
    connect(quantityGroupWidget, &AbstractEditorWidget::dataChanged,
            this, &PropQuantityGroup::setTabName);
    connect(quantityGroupWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);
}

PropQuantityGroup::~PropQuantityGroup()
{

}

void PropQuantityGroup::setModel(TreeXmlHashModel *model)
{
    quantityGroupWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropQuantityGroup::dataId(const QModelIndex &index)
{
    return modelData(DBQUANTITYGROUPXML::QUANTITYGROUP,
                               DBQUANTITYGROUPXML::ID,
                               index).toString();
}

void PropQuantityGroup::setCurrent(const QModelIndex &index)
{
    quantityGroupWidget->setCurrent(index);
}

void PropQuantityGroup::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString quantityGroupName = modelData(
                DBQUANTITYGROUPXML::QUANTITYGROUP,
                DBQUANTITYGROUPXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropQuantityGroup::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(quantityGroupName);
}

void PropQuantityGroup::edit(bool flag)
{
    quantityGroupWidget->edit(flag);
}

}}
