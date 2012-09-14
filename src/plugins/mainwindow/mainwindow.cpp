#include "mainwindow.h"
#include "pluginmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    createMainMenu();
    createConnection();
}

void MainWindow::createConnection()
{

}

void MainWindow::createMainMenu()
{
    QAction *pluginAbout = menuHelp->addAction(QString(tr("О модулях...")));
    pluginAbout->setIcon(QIcon());
    connect(pluginAbout, SIGNAL(triggered()), this, SLOT(showAboutPlugin()));
}

void MainWindow::showAboutPlugin()
{
    PluginManager *pluginManager = PluginManager::instance();
    pluginManager->showPluginView(this);
}
