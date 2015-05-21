#include "unitwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include <QStringListModel>
#include <QTreeView>
#include <QMessageBox>
#include "treefilterproxymodel.h"
#include "unitdelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

UnitWidget::UnitWidget(QWidget *parent) :
    AbstractModifyWidget(parent)
{
    setupUi(this);

    connect(toolButtonAddUnit,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteUnit,SIGNAL(clicked()),this,SLOT(remove()));
    AbstractModifyWidget::setItemView(tableViewUnit);
    itemView()->setItemDelegate(new UnitDelegate());
}

UnitWidget::~UnitWidget()
{
    delete itemView()->itemDelegate();
}

void UnitWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Наименование"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Код ОКЕИ"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Коэффициент"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Разница"));
    proxyModel()->setHeaderData(4,  Qt::Horizontal, tr("Обозначение"));
    proxyModel()->setHeaderData(5,  Qt::Horizontal, tr("Символы"));
    proxyModel()->setHeaderData(6,  Qt::Horizontal, tr("Обозначение (межд.)"));
    proxyModel()->setHeaderData(7,  Qt::Horizontal, tr("Символы (межд.)"));
    proxyModel()->setHeaderData(8,  Qt::Horizontal, tr("Сущность ЕИ"));
    proxyModel()->setHeaderData(9,  Qt::Horizontal, tr("Идентификатор"));
    itemView()->setModel(proxyModel());

    for (int column = 8; column < 16; ++column)
        tableViewUnit->setColumnHidden(column,true);

    tableViewUnit->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableViewUnit->horizontalHeader()->setDefaultSectionSize(75);
}

void UnitWidget::add()
{
    if (AbstractModifyWidget::add(DBUNITXML::UNIT)) {
        QModelIndex index = itemView()->currentIndex();
        proxyModel()->setData(index.sibling(
                                 index.row(),
                                 model()->columnDisplayedAttr(
                                     DBUNITXML::UNIT,
                                     DBUNITXML::COEFF)
                                 ),
                             1);
        proxyModel()->setData(index.sibling(
                                 index.row(),
                                 model()->columnDisplayedAttr(
                                     DBUNITXML::UNIT,
                                     DBUNITXML::DELTA)
                                 ),
                             0);
        proxyModel()->setData(index.sibling(
                                 index.row(),
                                 model()->columnDisplayedAttr(
                                     DBUNITXML::UNIT,
                                     DBUNITXML::CODE)
                                 ),
                             0);
    }
}

void UnitWidget::edit(bool flag)
{
    AbstractModifyWidget::edit();

    toolButtonAddUnit->setEnabled(flag);
    toolButtonDeleteUnit->setEnabled(flag);
}

}}
