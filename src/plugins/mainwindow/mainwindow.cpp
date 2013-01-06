#include <QCloseEvent>

#include "mainwindow.h"
#include <plugin/pluginmanager.h>

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt)
{
    setName("Главное окно");
    setDescript(tr("Главное окно"));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");

    setupUi(this);
    connect(actionWindowClose, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
    connect(actionWindowCloseAll, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    connect(actionWindowCascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
    connect(actionWindowTile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    connect(actionWindowNext, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    connect(actionWindowPrev, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
    connect(actionWindowGui, SIGNAL(triggered(bool)), this, SLOT(setWindowModeEnable(bool)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
}

bool MainWindow::initialize()
{
    show();

    return true;
}

bool MainWindow::release()
{
    return true;
}

void MainWindow::readSettings()
{
    resize(settings()->value("size", QSize(687, 582)).toSize());
    move(settings()->value("pos", QPoint(200, 200)).toPoint());
    if (settings()->value("viewMode")== QMdiArea::SubWindowView)
        setWindowModeEnable(true);
    else
        setWindowModeEnable(false);
}

void MainWindow::writeSettings()
{
    settings()->setValue("size", size());
    settings()->setValue("pos", pos());
    settings()->setValue("viewMode", mdiArea->viewMode());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}

void MainWindow::setWindowModeEnable(bool mode)
{
    if (mode)
    {
        mdiArea->setViewMode(QMdiArea::SubWindowView);
        if (mdiArea->activeSubWindow() != NULL)
            mdiArea->activeSubWindow()->showMaximized();
    }
    else
        mdiArea->setViewMode(QMdiArea::TabbedView);
    updateMenus();
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (mdiArea->subWindowList().count()>0);
    actionWindowClose->setEnabled(hasMdiChild);
    actionWindowCloseAll->setEnabled(hasMdiChild);
    actionWindowCascade->setEnabled(hasMdiChild&&mdiArea->viewMode() == QMdiArea::SubWindowView);
    actionWindowTile->setEnabled(hasMdiChild&&mdiArea->viewMode() == QMdiArea::SubWindowView);
    actionWindowNext->setEnabled(hasMdiChild);
    actionWindowPrev->setEnabled(hasMdiChild);
    actionWindowGui->setEnabled(hasMdiChild);
    actionWindowGui->setChecked(mdiArea->viewMode() == QMdiArea::SubWindowView);
}

QMdiSubWindow* MainWindow::addSubWindow(QWidget* widget)
{
    if (mdiArea->setActiveSubWindow(widget->objectName()))
    {
        return NULL;
    }

    QMdiSubWindow *subWindow = new QMdiSubWindow;
    subWindow->setWidget(widget);
    mdiArea->addSubWindow(subWindow);
    widget->setParent(subWindow);
    return subWindow;
}

QMdiSubWindow* MainWindow::setActiveSubWindow(QString objName)
{
    return mdiArea->setActiveSubWindow(objName);
}

QMdiSubWindow* MainWindow::subWindow(QString objName)
{
    return  mdiArea->subWindow(objName);
}

QList<QMdiSubWindow *> MainWindow::subWindowList() const
{
    return mdiArea->subWindowList();
}

QMenu *MainWindow::getMenuFile()
{
    return menuFile;
}

QMenu *MainWindow::getMenuEdit()
{
    return menuEdit;
}

QMenu *MainWindow::getMenuSettings()
{
    return menuSettings;
}

QMenu *MainWindow::getMenuHelp()
{
    return menuHelp;
}

QToolBar *MainWindow::getToolBarMain()
{
    return toolBarMain;
}

MdiExtArea *MainWindow::getMdiArea()
{
    return mdiArea;
}


#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
