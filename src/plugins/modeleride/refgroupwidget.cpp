#include "refgroupwidget.h"

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefGroupWidget::RefGroupWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,  SIGNAL(clicked()), this, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), this, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), this, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), this, SLOT(submit()));
}

RefGroupWidget::~RefGroupWidget()
{

}

void RefGroupWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditRefGroupName,
                             model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                        DBREFGROUPXML::NAME));
    dataMapper()->addMapping(lineEditRefGroupAlias,
                             model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                        DBREFGROUPXML::ALIAS));
}

bool RefGroupWidget::isEmpty()
{
    return lineEditRefGroupName->text().isEmpty();
}

void RefGroupWidget::add()
{
    AbstractEditorWidget::add(DBREFGROUPXML::REFGROUP);
}

void RefGroupWidget::edit(bool flag)
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

void RefGroupWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,
                lineEditRefGroupName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа справочника с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

}}
