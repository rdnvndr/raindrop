#include "classtreeview.h"

ClassTreeView::ClassTreeView(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    treeView->resize(10,10);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
}
