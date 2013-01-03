#include "mainwindowplug.h"


MainWindowPlug::MainWindowPlug(QObject *parent):
    IPlugin(parent)
{
    setName("MainWindow");
    setDescript(tr("Главное окно"));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");

    window = new MainWindow();
    publicObject(window);
    publicObject(window->menuFile);
    publicObject(window->menuEdit);
    publicObject(window->menuSettings);
    publicObject(window->menuHelp);
    publicObject(window->toolBarMain);
    publicObject(window->mdiArea);
}

MainWindowPlug::~MainWindowPlug()
{
    delete window;
}

bool MainWindowPlug::initialize()
{
    window->show();

    return true;
}

bool MainWindowPlug::release()
{
    return true;
}

void MainWindowPlug::readSettings()
{
    window->resize(settings()->value("size", QSize(687, 582)).toSize());
    window->move(settings()->value("pos", QPoint(200, 200)).toPoint());
    if (settings()->value("viewMode")== QMdiArea::SubWindowView)
        window->setWindowModeEnable(true);
    else
        window->setWindowModeEnable(false);
}

void MainWindowPlug::writeSettings()
{
    settings()->setValue("size", window->size());
    settings()->setValue("pos", window->pos());
    settings()->setValue("viewMode", window->mdiArea->viewMode());
}
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(mainwindow, MainWindowPlug)
#endif
