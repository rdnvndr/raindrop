#include "treefilterwidget.h"
TreeFilterWidget::TreeFilterWidget(QWidget* pwgt) : QWidget(pwgt) {
    setupUi(this);

    treeWidget->setIconSize(QSize(20, 20));
    treeWidget->setFrameStyle(QFrame::NoFrame);
    treeWidget->setHeaderHidden(true);
    treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeWidget->setIndentation(10);
    treeWidget->setStyleSheet("QTreeWidget {background: palette(Window);}");
    treeWidget->resize(10,10);
    connect(lineEditFilter, SIGNAL(textChanged(QString)), treeWidget, SLOT(searchShowAllItem(QString)));

}
