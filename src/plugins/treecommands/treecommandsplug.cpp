#include <QMainWindow>

#include <pluginmanager.h>
#include <treedockwidget/treedockwidget.h>
#include "treecommandsplug.h"


TreeCommandsPlug::TreeCommandsPlug(QObject *parent):
    IPlugin(parent)
{
    setName("TreeCommands");
    setDescript(tr("Дерево команд"));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");
    addDepend("MainWindowPlug");
}

bool TreeCommandsPlug::initialize()
{
    PluginManager* pluginManager = PluginManager::instance();
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(pluginManager->getObjectByName(
                                               "MainWindowPlug::MainWindow"));

    TreeDockWidget* dockWidget = new TreeDockWidget(mainWindow);
    mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
    dockWidget->setWindowTitle(tr("Список команд"));
    dockWidget->show();
    dockWidget->setObjectName("TreeDockWidget");
    publicObject(dockWidget);

    return true;
}

bool TreeCommandsPlug::release()
{
    return true;
}

void TreeCommandsPlug::readSettings()
{

}

void TreeCommandsPlug::writeSettings()
{

}

Q_EXPORT_PLUGIN2(treecommands, TreeCommandsPlug)
