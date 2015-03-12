#include "filterwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QHeaderView>
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;

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

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));
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
