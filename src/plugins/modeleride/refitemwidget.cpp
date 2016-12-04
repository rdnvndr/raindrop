#include "refitemwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "refitemdelegate.h"
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefItemWidget::RefItemWidget(QWidget *parent) :
    AbstractModifyWidget(parent)
{
    setupUi(this);

    connect(toolButtonAddClass, &QToolButton::clicked,
            this, &RefItemWidget::addLinkToClass);
    connect(toolButtonAddFilter, &QToolButton::clicked,
            this, &RefItemWidget::addLinkToFilter);
    connect(toolButtonAddRef, &QToolButton::clicked,
            this, &RefItemWidget::addLinkToRef);
    connect(toolButtonDelete, &QToolButton::clicked,
            this, &RefItemWidget::remove);

    AbstractModifyWidget::setItemView(treeView);
    itemView()->setItemDelegate(new RefItemDelegate());
}

RefItemWidget::~RefItemWidget()
{
    delete itemView()->itemDelegate();
}

void RefItemWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Псевдоним"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Класс/Справочник/Фильтр"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Родитель"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));

    for (qint32 column =  proxyModel()->columnCount() - 1; column > 1; --column)
        treeView->setColumnHidden(column,true);

    treeView->header()->setDefaultAlignment(Qt::AlignCenter);
    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setDefaultSectionSize(200);
}

void RefItemWidget::addLinkToClass()
{
    QModelIndex srcIndex = treeView->currentIndex();
    if (srcIndex.data(TreeXmlModel::TagRole) != DBLINKTOFILTERXML::LINKTOFILTER)
        srcIndex = srcIndex.parent();

    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();
    QModelIndex index = proxyModel()->insertLastRows(0,1,srcIndex);

    if (index.isValid()) {
        proxyModel()->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::addLinkToRef()
{
    QModelIndex srcIndex = treeView->currentIndex();
    if (srcIndex.data(TreeXmlModel::TagRole) != DBLINKTOFILTERXML::LINKTOFILTER)
        srcIndex = srcIndex.parent();

    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();
    QModelIndex index = proxyModel()->insertLastRows(0,1,srcIndex);

    if (index.isValid()) {
        proxyModel()->setData(index, DBLINKTOREFXML::LINKTOREF, TreeXmlModel::TagRole);
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::addLinkToFilter()
{
    QModelIndex srcIndex = treeView->currentIndex();
    if (srcIndex.data(TreeXmlModel::TagRole) == DBLINKTOFILTERXML::LINKTOFILTER)
        srcIndex = srcIndex.parent();

    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();
    QModelIndex index = proxyModel()->insertLastRows(0,1,srcIndex);

    if (index.isValid()) {
        proxyModel()->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::edit(bool flag)
{
    treeView->setCurrentIndex(QModelIndex());
    toolButtonAddClass->setEnabled(flag);
    toolButtonAddRef->setEnabled(flag);
    toolButtonAddFilter->setEnabled(flag);
    toolButtonDelete->setEnabled(flag);
    proxyModel()->setEditable(flag);
}

}}
