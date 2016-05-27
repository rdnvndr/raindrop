#include "quantitygroupwidget.h"

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

QuantityGroupWidget::QuantityGroupWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,  SIGNAL(clicked()), this, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), this, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), this, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), this, SLOT(submit()));
}

QuantityGroupWidget::~QuantityGroupWidget()
{

}

void QuantityGroupWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditQuantityGroupName,
                         model->columnDisplayedAttr(DBQUANTITYGROUPXML::QUANTITYGROUP,
                                                    DBQUANTITYXML::NAME));
    dataMapper()->addMapping(lineEditQuantityGroupAlias,
                         model->columnDisplayedAttr(DBQUANTITYGROUPXML::QUANTITYGROUP,
                                                   DBQUANTITYGROUPXML::ALIAS));
}

bool QuantityGroupWidget::isEmpty()
{
    return lineEditQuantityGroupName->text().isEmpty();
}

void QuantityGroupWidget::add()
{
    AbstractEditorWidget::add(DBQUANTITYGROUPXML::QUANTITYGROUP);
}

void QuantityGroupWidget::edit(bool flag)
{
    if (groupBoxQuantityGroup->isEnabled()==flag)
        return;

    if (isEmpty()) {
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    groupBoxQuantityGroup->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);

    emit edited(flag);
}

void QuantityGroupWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBQUANTITYGROUPXML::QUANTITYGROUP,
                DBQUANTITYGROUPXML::NAME,
                lineEditQuantityGroupName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа величины с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

}}