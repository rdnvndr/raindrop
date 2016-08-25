#include "treedockwidget.h"
#include <QMainWindow>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace TreeDockWidget {

TreeDockWidget::TreeDockWidget(QWidget *parent) :
    QDockWidget(parent), IPlugin("IMainWindow")
{
    this->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    this->setMinimumSize(QSize(160, 0));

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    stackedWidget = new QStackedWidget(this);

    verticalLayout->addWidget(stackedWidget);

    toolbar = new QToolBar();
    toolbar->setIconSize(QSize(20, 20));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    if (QApplication::style()->objectName() == "windows")
        toolbar->setStyleSheet(toolbar->styleSheet() +
                "QToolBar {"
                        "border-left: 1px solid palette(light);"
                        "border-top: 1px solid palette(light);"
                        "border-right: 1px solid palette(mid);"
                        "border-bottom: 1px solid palette(mid);"
                "}");
    verticalLayout->addWidget(toolbar);

    QWidget *qw = new QWidget();
    qw->setLayout(verticalLayout);
    this->setWidget(qw);
    toolbar->show();

    PluginManager *pluginManager = PluginManager::instance();
    QMainWindow *mainWindow = qobject_cast<QMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    if (QApplication::style()->objectName() == "windows")
        mainWindow->setStyleSheet(mainWindow->styleSheet()+
                "QDockWidget::title {"
                        "border-left: 1px solid palette(light);"
                        "border-top: 1px solid palette(light);"
                        "border-right: 1px solid palette(mid);"
                        "border-bottom: 1px solid palette(mid);"
                        "text-align: left;"
                        "padding-left: 5px;"
                        "}"
                        "QMainWindow::separator {"
                        "border-left: 1px solid palette(light);"
                        "border-top: 1px solid palette(light);"
                        "border-right: 1px solid palette(mid);"
                        "border-bottom: 1px solid palette(mid);"
                "}");

    mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), this);
    this->setWindowTitle(tr("Список команд"));
    this->show();
    this->setObjectName("TreeDockWidget");
}

void TreeDockWidget::insertWidget(QIcon icon,QString name, QWidget *widget){
    stackedWidget->addWidget(widget);
    QAction *button = toolbar->addAction(icon, name);
    stackedTree[button] = widget;
    connect(button, SIGNAL(triggered()), this, SLOT(setActionTreeWidget()));
}

QTreeWidget *TreeDockWidget::insertTreeWidget(QIcon icon,QString name){
    TreeFilterWidget *treeWidget = new TreeFilterWidget();
    insertWidget(icon,name, treeWidget);
    connect(treeWidget->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, qint32)),
                this, SLOT(callFuncTreeWidget(QTreeWidgetItem*, qint32)));
    return treeWidget->treeWidget;
}

void TreeDockWidget::setActionTreeWidget(){
    QAction *action = qobject_cast<QAction*>(sender());
    stackedWidget->setCurrentWidget(stackedTree[action]);
}

void TreeDockWidget::callFuncTreeWidget(QTreeWidgetItem *item, qint32 column){
    Q_UNUSED(column)

    QMetaObject::invokeMethod(objTreeItem[item],funcTreeItem[item]);
}

void TreeDockWidget::setFuncTreeItem(QTreeWidgetItem *item, QObject *obj, const char *funcname){
    funcTreeItem[item] = funcname;
    objTreeItem[item] = obj;
}

}}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(treedockwidget, TreeDockWidget)
#endif
