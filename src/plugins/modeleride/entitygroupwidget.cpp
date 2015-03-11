#include "entitygroupwidget.h"

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

EntityGroupWidget::EntityGroupWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,  SIGNAL(clicked()), this, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), this, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), this, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), this, SLOT(submit()));
}

EntityGroupWidget::~EntityGroupWidget()
{

}

void EntityGroupWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditEntityGroupName,
                         model->columnDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                                    DBENTITYXML::NAME));
    dataMapper()->addMapping(lineEditEntityGroupDesc,
                         model->columnDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                                   DBENTITYGROUPXML::DESCRIPTION));
}

bool EntityGroupWidget::isEmpty()
{
    return lineEditEntityGroupName->text().isEmpty();
}

void EntityGroupWidget::add()
{
    AbstractEditorWidget::add(DBENTITYGROUPXML::ENTITYGROUP);
}

void EntityGroupWidget::edit(bool flag)
{
    if (groupBoxEntityGroup->isEnabled()==flag)
        return;

    if (isEmpty()) {
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    groupBoxEntityGroup->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);

    emit edited(flag);
}

void EntityGroupWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBENTITYGROUPXML::ENTITYGROUP,
                DBENTITYGROUPXML::NAME,
                lineEditEntityGroupName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа сущность ЕИ с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

}}
