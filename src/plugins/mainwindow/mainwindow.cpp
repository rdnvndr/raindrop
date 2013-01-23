#include <QCloseEvent>

#include "mainwindow.h"
#include "menubar.h"
#include <plugin/pluginmanager.h>
#include <QUuid>

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt)
{
    setName(tr("Главное окно"));
    setDescript(tr("Главное окно"));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");

    this->setMenuBar(new MenuBar());

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

MainWindow::~MainWindow()
{
    settings()->beginGroup("IMainWindow");
    writeSettings();
    settings()->endGroup();
    settings()->sync();
}

bool MainWindow::initialize()
{
    settings()->beginGroup("IMainWindow");
    readSettings();
    settings()->endGroup();

    addAction(tr("Главное окно"),actionExit);
    addAction(tr("Главное окно"),actionWindowCascade);
    addAction(tr("Главное окно"),actionWindowClose);
    addAction(tr("Главное окно"),actionWindowCloseAll);
    addAction(tr("Главное окно"),actionWindowGui);
    addAction(tr("Главное окно"),actionWindowNext);
    addAction(tr("Главное окно"),actionWindowPrev);
    addAction(tr("Главное окно"),actionWindowTile);

    show();
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

    readMenuSettings();
}

void MainWindow::writeSettings()
{
    // MainWindow settings
    settings()->setValue("size", size());
    settings()->setValue("pos", pos());
    settings()->setValue("viewMode", mdiArea->viewMode());

    // MenuBar settings
    menuArrayIndex = 0;
    settings()->beginWriteArray("MenuBar");
    writeMenuSettings(this->menuBar());
    settings()->endArray();
}

void MainWindow::addAction(QString category, QAction *action)
{
    m_actions.insert(category,action);
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

void MainWindow::writeMenuSettings(QWidget *menu, int level)
{
    foreach (QAction *child, menu->actions()) {
        settings()->setArrayIndex(menuArrayIndex);
        menuArrayIndex++;

        settings()->setValue("level", level);

        if (child->isSeparator()) {
            settings()->setValue("type", "Separator");
            settings()->setValue("name",  QUuid::createUuid().toString());
        } else if (child->menu()) {
            settings()->setValue("type", "Menu");
            settings()->setValue("name",  QUuid::createUuid().toString());
        } else {
            settings()->setValue("type", "Action");
            settings()->setValue("name", child->objectName());
        }
        settings()->setValue("text", child->text());

        //settings()->setValue("icon", child->icon());

        if (child->menu())
            writeMenuSettings(child->menu(), level+1);
    }

}

void MainWindow::readMenuSettings()
{
    //return;
/*
    QMenu *parentMenu = NULL;
    QMenu *currentMenu = NULL;
*/
    int prevLevel = 0;
    m_item = new MenuItem;
    MenuItem *parentItem  = m_item;
    MenuItem *currentItem = m_item;

    int size = settings()->beginReadArray("MenuBar");
    for (int i = 0; i < size; ++i) {
        settings()->setArrayIndex(i);
        int level = settings()->value("level").toInt();
        QString name = settings()->value("name").toString();
        QString text = settings()->value("text").toString();
        QString typeAction = settings()->value("type").toString();

        if (prevLevel<level)
            parentItem = currentItem;

        if (prevLevel>level)
            parentItem = parentItem->parentItem;

        currentItem = new MenuItem;
        currentItem->name = name;
        currentItem->text = text;
        currentItem->type = typeAction;
        currentItem->parentItem = parentItem;
        parentItem->childIItems.append(currentItem);
        m_actionItem[name] = currentItem;
/*
        if (prevLevel<level)
            parentMenu = currentMenu;

        if (prevLevel>level)
            parentMenu = qobject_cast<QMenu *>(parentMenu->parentWidget());

        if (typeAction == "Separator")
            if (parentMenu)
                parentMenu->addSeparator();
            else
                menuBar()->addSeparator();
        else if (typeAction == "Menu") {

           currentMenu = new QMenu(text);
           if (parentMenu)
                parentMenu->addMenu(currentMenu);
            else
                menuBar()->addMenu(currentMenu);
        } else {
            if (parentMenu)
                parentMenu->addAction(text);
            else
                menuBar()->addAction(text);
        }
*/
        prevLevel = level;
    }
    settings()->endArray();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
