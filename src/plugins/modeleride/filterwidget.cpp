#include "filterwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QHeaderView>
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

FilterWidget::FilterWidget(QWidget *parent) :
    AbstractItemWidget(parent)
{
    setupUi(this);

    QStringList tags;
    tags << DBFILTERXML::FILTER;
    proxyModel()->setAttributeTags(tags);

    AbstractItemWidget::setItemView(tableViewFilter);

    connect(toolButtonAdd, &QToolButton::clicked, this, &FilterWidget::add);
    connect(toolButtonDelete, &QToolButton::clicked, this, &FilterWidget::remove);

    connect(toolButtonEdit, &QToolButton::clicked, this, &FilterWidget::edit);
    connect(checkBoxInInherited, &QCheckBox::clicked, this, &FilterWidget::showParent);
}

FilterWidget::~FilterWidget()
{

}

void FilterWidget::setModel(TreeXmlHashModel *model)
{
    AbstractItemWidget::setModel(model);

    proxyModel()->setHeaderData(0, Qt::Horizontal, tr("Имя фильтра"));
    proxyModel()->setHeaderData(1, Qt::Horizontal, tr("Псевдоним"));
    proxyModel()->setHeaderData(2, Qt::Horizontal, tr("Первый класс"));
    proxyModel()->setHeaderData(3, Qt::Horizontal, tr("Второй класс"));
    proxyModel()->setHeaderData(4, Qt::Horizontal, tr("Описание прямое"));
    proxyModel()->setHeaderData(5, Qt::Horizontal, tr("Описание обратное"));
    proxyModel()->setHeaderData(6, Qt::Horizontal, tr("Индетификатор"));
    proxyModel()->setColumnCount(7);
    proxyModel()->setDynamicSortFilter(true);

    tableViewFilter->setSortingEnabled(true);
    tableViewFilter->sortByColumn(0,Qt::AscendingOrder);
    tableViewFilter->setColumnHidden(6,true);
}

void FilterWidget::add()
{
    AbstractItemWidget::add(DBFILTERXML::FILTER);
}

void FilterWidget::edit(bool flag)
{
    if (flag) {
        QModelIndex index = proxyModel()->mapToSource(tableViewFilter->currentIndex());
        emit dataEdited(index);
    }
}

bool FilterWidget::isEdit()
{
    return false;
}

void FilterWidget::showParent(bool flag)
{
    AbstractItemWidget::showParent(flag, DBCLASSXML::CLASS, DBCLASSXML::ID,
                                   DBFILTERXML::FILTER, DBFILTERXML::PARENT);

}

}}
