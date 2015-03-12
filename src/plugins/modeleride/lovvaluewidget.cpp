#include "lovvaluewidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "lovdelegate.h"
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

LovValueWidget::LovValueWidget(QWidget *parent) :
    AbstractModifyWidget(parent)
{
    setupUi(this);

    connect(toolButtonAddLovValue,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteLovValue,SIGNAL(clicked()),this,SLOT(remove()));

    AbstractModifyWidget::setItemView(tableViewLovValue);
    itemView()->setItemDelegate(new LovDelegate());
}

LovValueWidget::~LovValueWidget()
{
    delete itemView()->itemDelegate();
}

void LovValueWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Наименование"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Значение"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Список значений"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));

    for (int column = 2; column < 16; column++)
        tableViewLovValue->setColumnHidden(column,true);
}

void LovValueWidget::add()
{
    AbstractModifyWidget::add(DBLOVVALUEXML::LOVVALUE);
}

void LovValueWidget::edit(bool flag)
{
    AbstractModifyWidget::edit(flag);
    toolButtonAddLovValue->setEnabled(flag);
    toolButtonDeleteLovValue->setEnabled(flag);
}

}}
