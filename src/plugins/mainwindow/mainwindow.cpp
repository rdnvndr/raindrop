#include <QCloseEvent>
#include <QString>
#include "mainwindow.h"
#include "menubar.h"
#include "menu.h"
#include "toolbar.h"

#include <plugin/pluginmanager.h>
#include <QUuid>

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt), IPlugin("")
{
    setupUi(this);
    this->setMenuBar(new MenuBar());
    //writeMenuSettings();
    m_optionsDialog = NULL;

    actionWindowClose = new QAction(QIcon(), tr("Закрыть"), this);
    actionWindowClose->setObjectName("actionWindowClose");

    actionWindowCloseAll = new QAction(QIcon(), tr("Закрыть все"), this);
    actionWindowCloseAll->setObjectName("actionWindowCloseAll");

    actionWindowCascade = new QAction(QIcon(), tr("Каскадом"), this);
    actionWindowCascade->setObjectName("actionWindowCascade");

    actionWindowTile = new QAction(QIcon(), tr("Плиткой"), this);
    actionWindowTile->setObjectName("actionWindowTile");

    actionWindowNext = new QAction(QIcon(), tr("Следующее"), this);
    actionWindowNext->setObjectName("actionWindowNext");

    actionWindowPrev = new QAction(QIcon(), tr("Предыдущее"), this);
    actionWindowPrev->setObjectName("actionWindowPrev");

    actionWindowGui = new QAction(QIcon(), tr("Оконный вид"), this);
    actionWindowGui->setCheckable(true);
    actionWindowGui->setObjectName("actionWindowGui");

    actionGuiOptions = new QAction(QIcon(":guioptions"), tr("Оформление"), this);
    actionGuiOptions->setObjectName("actionGuiOptions");

    actionExit = new QAction(QIcon(":exit"), tr("Выход"), this);
    actionExit->setObjectName("actionExit");

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionWindowClose, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
    connect(actionWindowCloseAll, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    connect(actionWindowCascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
    connect(actionWindowTile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    connect(actionWindowNext, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    connect(actionWindowPrev, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
    connect(actionWindowGui, SIGNAL(triggered(bool)), this, SLOT(setWindowModeEnable(bool)));
    connect(actionGuiOptions, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

    readBarSettings();
    addAction(tr("Файл"),actionExit);
    addAction(tr("Окно"),actionWindowCascade);
    addAction(tr("Окно"),actionWindowClose);
    addAction(tr("Окно"),actionWindowCloseAll);
    addAction(tr("Окно"),actionWindowGui);
    addAction(tr("Окно"),actionWindowNext);
    addAction(tr("Окно"),actionWindowPrev);
    addAction(tr("Окно"),actionWindowTile);
    addAction(tr("Настройка"),actionGuiOptions);

    Menu *newMenu = new Menu("Новое меню");
    newMenu->setIcon(QIcon(":menu"));
    addAction(tr("Новое меню"),newMenu->menuAction());
    newMenu->menuAction()->setObjectName("actionNewMenu");

    QAction *newSeparator = new QAction("Разделитель",this);
    newSeparator->setIcon(QIcon(":separator"));
    newSeparator->setSeparator(true);
    addAction(tr("Новое меню"),newSeparator);
    newSeparator->setObjectName("actionNewSeparator");

    PluginManager* pluginManager = PluginManager::instance();
    connect(pluginManager,SIGNAL(endLoadingPlugins()),
            this,SLOT(readSettings()));
    show();
}

MainWindow::~MainWindow()
{
    writeSettings();
    settings()->sync();

    foreach (MenuItem *item, m_item) {
        cleanBranchAction(item);
        removeBranchAction(item);
    }
    m_item.clear();

    foreach (MenuItemHash actionItem, m_actionItem)
        actionItem.clear();
    m_actionItem.clear();

    m_actions.clear();

    delete actionWindowClose;
    delete actionWindowCloseAll;
    delete actionWindowCascade;
    delete actionWindowTile;
    delete actionWindowNext;
    delete actionWindowPrev;
    delete actionWindowGui;
    delete actionGuiOptions;
    delete actionExit;
}

void MainWindow::readSettings()
{
    settings()->beginGroup("IMainWindow");
    if (settings()->value("viewMode")== QMdiArea::SubWindowView)
        setWindowModeEnable(true);
    else
        setWindowModeEnable(false);
    restoreGeometry(settings()->value("geometry").toByteArray());
    restoreState(settings()->value("state").toByteArray());
    settings()->endGroup();
}

void MainWindow::writeSettings()
{
    // MainWindow settings
    settings()->beginGroup("IMainWindow");
    settings()->setValue("viewMode", mdiArea->viewMode());
    settings()->setValue("geometry", saveGeometry());
    settings()->setValue("state", saveState());
    settings()->endGroup();
}

void MainWindow::removeBranchAction(MenuItem *menuItem)
{
    if (menuItem) {
        foreach (MenuItem *item, menuItem->childItems) {
            removeBranchAction(item);
        }
        delete menuItem;
    }
}

void MainWindow::cleanBranchAction(MainWindow::MenuItem *menuItem)
{
    if (menuItem)
        foreach (MenuItem *item, menuItem->childItems){
            if (item->action)
            {
                if (item->action->isSeparator() || item->action->menu())
                    delete item->action;
                item->action = NULL;
            }
            cleanBranchAction(item);
        }
}

QAction *MainWindow::createBranchAction(MenuItem *menuItem)
{
    if (!menuItem)
        return NULL;

    if (menuItem->action && menuItem->type == "Menu")
        return menuItem->action;

    MenuItem *parentItem = menuItem->parentItem;
    if (!parentItem)
        return NULL;

    // Сортировка меню
    QAction *parentAction  = createBranchAction(parentItem);
    QMenu *parentMenu = (parentAction) ? parentAction->menu():NULL;
    ToolBar *toolBar = NULL;
    if (parentItem->type == "ToolBar") {
        toolBar = this->findChild<ToolBar *>(parentItem->name);
        if (!toolBar) {
            toolBar = new ToolBar();
            toolBar->setObjectName(parentItem->name);
            toolBar->setWindowTitle(parentItem->text);
            this->addToolBar(toolBar);
        }
    }

    QAction *prevAction = NULL;
    MenuItem *separatorItem = NULL;

    for (int row = parentItem->childItems.count()-1; row >= 0; row--) {
        MenuItem *item = parentItem->childItems.at(row);
        if (item == menuItem) {

            // Создание разделителя между двумя существующими QAction
            if (separatorItem) {
                prevAction = (parentMenu) ?
                            parentMenu->insertSeparator(prevAction)
                          : (toolBar) ?
                                toolBar->insertSeparator(prevAction)
                              : menuBar()->insertSeparator(prevAction);
                separatorItem->action = prevAction;
                prevAction->setObjectName(separatorItem->name);
            }

            break;
        }
        if (item->type == "Separator" && !item->action)
            separatorItem = item;

        if (item->action) {
            prevAction = item->action;
            separatorItem = NULL;
        }
    }

    // Создание пункта меню
    if (menuItem->type == "Menu") {
        Menu *currentMenu = new Menu(menuItem->text);
        QAction *currentAction =  NULL;
        if (parentMenu) {
            currentAction = (prevAction)
                    ? parentMenu->insertMenu(prevAction,currentMenu)
                    : parentMenu->addMenu(currentMenu);
        } else if (toolBar) {
            currentAction = currentMenu->menuAction();
            if (prevAction)
                toolBar->insertAction(prevAction,currentMenu->menuAction());
            else
                toolBar->addAction(currentMenu->menuAction());
        } else {
            currentAction = (prevAction)
                    ? menuBar()->insertMenu(prevAction,currentMenu)
                    : menuBar()->addMenu(currentMenu);
        }
        menuItem->action = currentAction;
        currentAction->setObjectName(menuItem->name);
        return currentAction;
    } else  if (menuItem->type =="Action" && menuItem->action != NULL) {
        if (parentMenu) {
            if (prevAction)
                parentMenu->insertAction(prevAction,menuItem->action);
            else
                parentMenu->addAction(menuItem->action);
        } else if (toolBar) {
            if (prevAction)
                toolBar->insertAction(prevAction,menuItem->action);
            else
                toolBar->addAction(menuItem->action);
        } else {
            if (prevAction)
                menuBar()->insertAction(prevAction,menuItem->action);
            else
                menuBar()->addAction(menuItem->action);
        }
    }

    return NULL;
}

void MainWindow::deleteBranchAction(MenuItem *menuItem)
{
    if (menuItem) {
        // Удаляtncz menuItem без детей
        if (menuItem->childItems.count()==0) {
            MenuItem *parentMenuItem = menuItem->parentItem;
            if (menuItem->action) {
                // Если Menu, то удаляем его иначе убираем QAction из него
                if (menuItem->action->menu())
                    delete menuItem->action;
                else if (parentMenuItem)
                    if (parentMenuItem->action)
                        if (parentMenuItem->action->menu())
                            parentMenuItem->action->menu()->removeAction(menuItem->action);
            }

            // Удаляем MenuItem из структуры
            if (parentMenuItem) {
                parentMenuItem->childItems.removeAll(menuItem);
                deleteBranchAction(parentMenuItem);
                delete menuItem;
            }
        }
    }
}

void MainWindow::addAction(QString category, QAction *action)
{
    m_actions.insert(category,action);

    QString name = action->objectName();

    foreach (MenuItemHash actionItem, m_actionItem)
        foreach (MenuItem *menuItem,actionItem.values(name)) {
            menuItem->action = action;
            createBranchAction(menuItem);
        }

    connect(action,SIGNAL(destroyed(QObject*)),
            this, SLOT(removeAction(QObject*)));
}

void MainWindow::removeAction(QObject *obj)
{
    foreach (const QString &category, m_actions.uniqueKeys())
        foreach (QAction* actionCategory, m_actions.values(category))
            if (obj == qobject_cast< QObject *>(actionCategory))
                m_actions.remove(category,actionCategory);

    QString name = obj->objectName();
    foreach (MenuItemHash actionItem, m_actionItem)
        foreach (MenuItem *item, actionItem.values(name)) {
            deleteBranchAction(item);
            actionItem.remove(name, item);
        }
}

void MainWindow::removeAction(QAction *action)
{
    removeAction(qobject_cast< QObject *>(action));
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

void MainWindow::refreshAllBar()
{
    QList<ToolBar *> toolBars = this->findChildren<ToolBar *> ();
    foreach (ToolBar* toolBar,toolBars) {
        this->removeToolBar(toolBar);
        delete toolBar;
    }
    this->setMenuBar(new MenuBar());

    foreach (MenuItem *item, m_item)
        cleanBranchAction(item);

    foreach (QAction* action, m_actions.values()) {
        if (action)
            foreach (MenuItemHash actionItem, m_actionItem)
                foreach (MenuItem *menuItem, actionItem.values(action->objectName())) {
                    menuItem->action = action;
                    createBranchAction(menuItem);
                }
    }
    readSettings();
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

MdiExtArea *MainWindow::getMdiArea()
{
    return mdiArea;
}

void MainWindow::showOptionsDialog()
{
    writeSettings();
    if (!m_optionsDialog) {
        m_optionsDialog = new MainWindowOptions(this);
        m_optionsDialog->createActionsModel(&m_actions);

        QMdiSubWindow *subWindow = addSubWindow(m_optionsDialog);

        connect(m_optionsDialog,SIGNAL(accepted()),
                this,SLOT(saveOptionsDialog()));
        connect(m_optionsDialog,SIGNAL(rejected()),
                this,SLOT(cancelOptionsDialog()));

        connect(m_optionsDialog->pushButtonCancel,SIGNAL(clicked()),
                subWindow,SLOT(close()));
        connect(m_optionsDialog->pushButtonSave,SIGNAL(clicked()),
                subWindow,SLOT(close()));

        connect(m_optionsDialog->pushButtonNew,SIGNAL(clicked()),
                this,SLOT(createToolBar()));
    } else
        addSubWindow(m_optionsDialog);
}

void MainWindow::saveOptionsDialog()
{
    writeBarSettings();
    m_optionsDialog = NULL;
}

void MainWindow::cancelOptionsDialog()
{
    refreshAllBar();
    m_optionsDialog = NULL;
}

void MainWindow::createToolBar()
{
    ToolBar *toolBar = new ToolBar();
    this->addToolBar(toolBar);
}

void MainWindow::writeMenu(QWidget *menu, int level)
{
    for (int row = 0;row < menu->actions().count(); row++) {
        QAction *child = menu->actions().at(row);

        if (child->isSeparator()
                && (row == 0 || row == menu->actions().count()-1))
            continue;

        settings()->setArrayIndex(m_menuArrayIndex);
        m_menuArrayIndex++;
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
            writeMenu(child->menu(), level+1);
    }
}

void MainWindow::writeBarSettings() {

    settings()->beginGroup("IMainWindow");

    m_menuArrayIndex = 0;
    settings()->beginWriteArray("BarSettings");

    settings()->setArrayIndex(m_menuArrayIndex);
    m_menuArrayIndex++;
    settings()->setValue("level", -1);
    settings()->setValue("type", "MenuBar");
    settings()->setValue("name",  QUuid::createUuid().toString());
    writeMenu(this->menuBar());

    const QList<ToolBar *> toolBars = this->findChildren<ToolBar *> ();
    foreach (ToolBar* toolBar,toolBars) {
        settings()->setArrayIndex(m_menuArrayIndex);
        m_menuArrayIndex++;
        settings()->setValue("level", -1);
        settings()->setValue("type", "ToolBar");
        QString toolBarName = QUuid::createUuid().toString();
        settings()->setValue("name", toolBarName);
        settings()->setValue("text", toolBar->windowTitle());
        toolBar->setObjectName(toolBarName);
        writeMenu(toolBar);
    }
    settings()->endArray();

    settings()->endGroup();
}

void MainWindow::readBarSettings()
{
    foreach (MenuItem *item, m_item) {
        cleanBranchAction(item);
        removeBranchAction(item);
    }
    m_item.clear();

    foreach (MenuItemHash actionItem, m_actionItem)
        actionItem.clear();
    m_actionItem.clear();
    m_actionItem.append(MenuItemHash());

    settings()->beginGroup("IMainWindow");

    int prevLevel = -1;
    MenuItem *parentItem  = NULL;
    MenuItem *currentItem = NULL;

    int size = settings()->beginReadArray("BarSettings");
    for (int i = 0; i < size; ++i) {
        settings()->setArrayIndex(i);
        int level = settings()->value("level").toInt();
        QString name = settings()->value("name").toString();
        QString text = settings()->value("text").toString();
        QString typeAction = settings()->value("type").toString();

        if (level < 0) {
            parentItem = NULL;
        } else {
            if (prevLevel<level)
                parentItem = currentItem;

            if (prevLevel>level)
                parentItem = parentItem->parentItem;
        }

        currentItem = new MenuItem;
        currentItem->name = name;
        currentItem->text = text;
        currentItem->type = typeAction;
        currentItem->action = NULL;
        currentItem->parentItem = parentItem;
        if (parentItem) {
            parentItem->childItems.append(currentItem);
            m_actionItem[0].insert(name,currentItem);
            prevLevel = level;
        } else {
            qDebug() << "Append" ;
            m_item.append(currentItem);
            parentItem = currentItem;
            prevLevel = 0;
        }

    }
    settings()->endArray();
    settings()->endGroup();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
