#include "treedockwidget.h"

TreeDockWidget::TreeDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    this->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    this->setMinimumSize(QSize(160, 0));

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    stackedWidget = new QStackedWidget(this);

    verticalLayout->addWidget(stackedWidget);

    toolbar = new QToolBar();
    toolbar->setIconSize(QSize(20, 20));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    verticalLayout->addWidget(toolbar);

    QWidget* qw = new QWidget();
    qw->setLayout(verticalLayout);
    this->setWidget(qw);
    toolbar->show();
}

void TreeDockWidget::insertWidget(QIcon icon,QString name, QWidget* widget){
    stackedWidget->addWidget(widget);
    QAction* button = toolbar->addAction(icon, name);
    stackedTree[button] = widget;
    connect(button, SIGNAL(triggered()), this, SLOT(setActionTreeWidget()));
}

QTreeWidget* TreeDockWidget::insertTreeWidget(QIcon icon,QString name){
    TreeFilterWidget* treeWidget = new TreeFilterWidget();
    insertWidget(icon,name, treeWidget);
    connect(treeWidget->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                this, SLOT(callFuncTreeWidget(QTreeWidgetItem*, int)));
    return treeWidget->treeWidget;
}

void TreeDockWidget::setActionTreeWidget(){
    QAction* action = qobject_cast<QAction*>(sender());
    stackedWidget->setCurrentWidget(stackedTree[action]);
}

void TreeDockWidget::callFuncTreeWidget(QTreeWidgetItem* item, int column){
    QMetaObject::invokeMethod(objTreeItem[item],funcTreeItem[item]);
}

void TreeDockWidget::setFuncTreeItem(QTreeWidgetItem* item, QObject* obj, const char* funcname){
    funcTreeItem[item] = funcname;
    objTreeItem[item] = obj;
}
