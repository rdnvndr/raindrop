#include "mainwindowplugin.h"

MainWindowPlugin::MainWindowPlugin(QObject *parent):
    IPlugin(parent)
{
    setName("MainWindow");
    setDescript(tr("Главное окно"));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");
}

MainWindowPlugin::~MainWindowPlugin()
{
}

bool MainWindowPlugin::initialize()
{
    m_mainWindows = new MainWindow();
    m_mainWindows->show();

    return true;
}

bool MainWindowPlugin::release()
{    
    return true;
}

void MainWindowPlugin::readSettings()
{
}

void MainWindowPlugin::writeSettings()
{
}

QMainWindow* MainWindowPlugin::mainWindow()
{
    return m_mainWindows.data();
}

Q_EXPORT_PLUGIN2(mainwindow, MainWindowPlugin)
