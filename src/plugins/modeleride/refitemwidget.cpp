#include "refitemwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "refitemdelegate.h"
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefItemWidget::RefItemWidget(QWidget *parent) :
    AbstractListEditorWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonAddIn,SIGNAL(clicked()),this,SLOT(addChild()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    AbstractListEditorWidget::setItemView(treeView);
    itemView()->setItemDelegate(new RefItemDelegate());
}

RefItemWidget::~RefItemWidget()
{

}

void RefItemWidget::setModel(TreeXmlHashModel *model)
{
    AbstractListEditorWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Псевдоним"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Класс/Фильтр"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Родитель"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));

    for (int column = 2; column < proxyModel()->columnCount(); column++)
        treeView->setColumnHidden(column,true);

}

void RefItemWidget::add()
{
    QModelIndex srcIndex = treeView->currentIndex().parent();
    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();

    QModelIndex index = proxyModel()->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        if (srcIndex.data(TreeXmlModel::TagRole)==DBLINKTOCLASSXML::LINKTOCLASS) {
            proxyModel()->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
        } else {
            proxyModel()->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        }
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::addChild()
{
    QModelIndex srcIndex = treeView->currentIndex();
    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();
    QModelIndex index = proxyModel()->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        if (srcIndex.data(TreeXmlModel::TagRole)==DBLINKTOCLASSXML::LINKTOCLASS) {
            proxyModel()->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
        } else {
            proxyModel()->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        }
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::edit(bool flag)
{

    treeView->setCurrentIndex(QModelIndex());

    toolButtonAdd->setEnabled(flag);
    toolButtonAddIn->setEnabled(flag);
    toolButtonDelete->setEnabled(flag);
    proxyModel()->setEditable(flag);
}

}}
