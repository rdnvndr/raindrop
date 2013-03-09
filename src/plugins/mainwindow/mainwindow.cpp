#include <QCloseEvent>
#include <QString>
#include <QWhatsThis>
#include <QUuid>
#include <plugin/pluginmanager.h>

#include "mainwindow.h"
#include "menubar.h"
#include "menu.h"
#include "toolbar.h"

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt), IPlugin("")
{
    setupUi(this);
    this->setMenuBar(new MenuBar());
    setIconSize(QSize(20,20));

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

    actionGuiOptions = new QAction(QIcon(":guioptions"), tr("Оформление..."), this);
    actionGuiOptions->setObjectName("actionGuiOptions");

    actionExit = new QAction(QIcon(":exit"), tr("Выход"), this);
    actionExit->setObjectName("actionExit");

    actionAboutQt = new QAction(QIcon(":qt"), tr("О Qt..."), this);
    actionAboutQt->setObjectName("actionAboutQt");

    actionWhatsThis = new QAction(QIcon(":whatsthis"), tr("Что это?"), this);
    actionWhatsThis->setObjectName("actionWhatsThis");
    actionWhatsThis->setShortcut(tr("Shift+F1"));

    connect(actionWhatsThis, SIGNAL(triggered()), this, SLOT(showWhatsThis()));
    connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
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
    addAction(tr("Справка"),actionWhatsThis);
    addAction(tr("Справка"),actionAboutQt);
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
            this,SLOT(endLoadingPlugins()));
}

MainWindow::~MainWindow()
{
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
    delete actionAboutQt;
    delete actionWhatsThis;
}

void MainWindow::readSettings()
{
    settings()->beginGroup("IMainWindow");
    if (settings()->value("viewMode")== QMdiArea::SubWindowView)
        setWindowModeEnable(true);
    else
        setWindowModeEnable(false);
    QPoint pos = settings()->value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings()->value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
    restoreState(settings()->value("state").toByteArray());
    settings()->endGroup();
}

void MainWindow::writeSettings()
{
    // MainWindow settings
    settings()->beginGroup("IMainWindow");
    settings()->setValue("viewMode", mdiArea->viewMode());
    settings()->setValue("pos", pos());
    settings()->setValue("size", size());
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
        QPixmap pixmap;
        pixmap.loadFromData(menuItem->icon);
        QIcon icon = QIcon(pixmap);
        currentMenu->setNativeIcon(menuItem->icon);
        currentAction->setIcon(icon);
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
    QKeySequence hotkey = m_hotkey.value(action->objectName());
    if (hotkey != QKeySequence())
        action->setShortcut(hotkey);

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
    writeSettings();
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

void MainWindow::refreshAllBar(bool readingBarSettings)
{
    QList<ToolBar *> toolBars = this->findChildren<ToolBar *> ();
    foreach (ToolBar* toolBar,toolBars) {
        this->removeToolBar(toolBar);
        delete toolBar;
    }
    this->setMenuBar(new MenuBar());

    if (readingBarSettings)
        readBarSettings();
    else
        foreach (MenuItem *item, m_item)
            cleanBranchAction(item);

    foreach (QAction* action, m_actions.values()) {
        if (action) {
            foreach (MenuItemHash actionItem, m_actionItem)
                foreach (MenuItem *menuItem, actionItem.values(action->objectName())) {
                    menuItem->action = action;
                    createBranchAction(menuItem);
                }
            QKeySequence hotkey = m_hotkey.value(action->objectName());
            if (hotkey != QKeySequence())
                action->setShortcut(hotkey);
        }

    }
    settings()->beginGroup("IMainWindow");
    restoreState(settings()->value("state").toByteArray());
    settings()->endGroup();
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

QMenu *MainWindow::createPopupMenu()
{
    QMenu *menu = QMainWindow::createPopupMenu();
    menu->addSeparator();
    menu->addAction(actionGuiOptions);
    return menu;
}

MdiExtArea *MainWindow::getMdiArea()
{
    return mdiArea;
}

void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

void MainWindow::showWhatsThis()
{
    QWhatsThis::enterWhatsThisMode();
}

void MainWindow::showOptionsDialog()
{
    settings()->beginGroup("IMainWindow");
    settings()->setValue("state", saveState());
    settings()->endGroup();
    if (!m_optionsDialog) {
        m_optionsDialog = new MainWindowOptions(this);
        m_optionsDialog->createActionsModel(&m_actions);
        m_optionsDialog->createToolBarModel(this);

        QMdiSubWindow *subWindow = addSubWindow(m_optionsDialog);
        connect(subWindow,SIGNAL(windowStateChanged(Qt::WindowStates,Qt::WindowStates)),
                this,SLOT(optionsDialogStateChanged(Qt::WindowStates,Qt::WindowStates)));
        setEditedAllMenu(true);

        connect(m_optionsDialog,SIGNAL(accepted()),
                this,SLOT(saveOptionsDialog()));
        connect(m_optionsDialog,SIGNAL(rejected()),
                this,SLOT(cancelOptionsDialog()));

        connect(m_optionsDialog->pushButtonCancel,SIGNAL(clicked()),
                subWindow,SLOT(close()));
        connect(m_optionsDialog->pushButtonSave,SIGNAL(clicked()),
                subWindow,SLOT(close()));
    } else
        addSubWindow(m_optionsDialog);

    m_optionsDialog->setIconSize(this->iconSize());
    m_optionsDialog->setIconStyle(this->toolButtonStyle());
}

void MainWindow::saveOptionsDialog()
{
    this->setIconSize(m_optionsDialog->iconSize());
    this->setToolButtonStyle(m_optionsDialog->iconStyle());
    writeBarSettings();
    settings()->sync();
    m_optionsDialog = NULL;
}

void MainWindow::cancelOptionsDialog()
{
    refreshAllBar();
    m_optionsDialog = NULL;
}

void MainWindow::optionsDialogStateChanged(Qt::WindowStates oldState,
                                           Qt::WindowStates newState)
{
    bool edited;
    if (newState.testFlag(Qt::WindowActive)) {
        if (!oldState.testFlag(Qt::WindowActive)) {
            edited = true;
        }
    } else {
        if (oldState.testFlag(Qt::WindowActive)) {
            edited = false;
        }
    }

    setEditedAllMenu(edited);
}

void MainWindow::endLoadingPlugins()
{
    readSettings();
    show();
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
            Menu *menu = qobject_cast<Menu *>(child->menu());
            settings()->setValue("icon", menu->nativeIcon());
        } else {
            settings()->setValue("type", "Action");
            settings()->setValue("name", child->objectName());
        }
        settings()->setValue("text", child->text());

        if (child->menu())
            writeMenu(child->menu(), level+1);
    }
}

void MainWindow::setEditedMenu(QWidget *widget, bool edited)
{
    MenuBar *menuBar = qobject_cast<MenuBar *>(widget);
    if (menuBar)
        menuBar->setEdited(edited);

    ToolBar *toolBar = qobject_cast<ToolBar *>(widget);
    if (toolBar)
        toolBar->setEdited(edited);

    Menu *menu = qobject_cast<Menu *>(widget);
    if (menu)
        menu->setEdited(edited);

    for (int row = 0;row < widget->actions().count(); row++) {
        QAction *child = widget->actions().at(row);
        if (child->menu())
            setEditedMenu(child->menu(),edited);
    }
}

void MainWindow::setEditedAllMenu(bool edited)
{
    setEditedMenu(this->menuBar(), edited);

    const QList<ToolBar *> toolBars = this->findChildren<ToolBar *> ();
    foreach (ToolBar* toolBar,toolBars) {
        setEditedMenu(toolBar, edited);
    }
}

void MainWindow::writeBarSettings() {

    settings()->beginGroup("IMainWindow");

    settings()->setValue("IconSize",this->iconSize());
    settings()->setValue("IconStyle",this->toolButtonStyle());

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

    settings()->beginWriteArray("HotKeySettings");
    m_menuArrayIndex = 0;
    foreach (QAction* action, m_actions.values())
        if (action)
            if (action->shortcut() != QKeySequence()){
                settings()->setArrayIndex(m_menuArrayIndex);
                m_menuArrayIndex++;
                settings()->setValue("name", action->objectName());
                settings()->setValue("hotkey",action->shortcut());
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

    this->setIconSize(settings()->value("IconSize").value<QSize>());
    Qt::ToolButtonStyle style =
            (Qt::ToolButtonStyle)settings()->value("IconStyle").toInt();
    this->setToolButtonStyle(style);

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
        currentItem->icon = settings()->value("icon").toByteArray();
        currentItem->type = typeAction;
        currentItem->action = NULL;
        currentItem->parentItem = parentItem;
        if (parentItem) {
            parentItem->childItems.append(currentItem);
            m_actionItem[0].insert(name,currentItem);
            prevLevel = level;
        } else {
            m_item.append(currentItem);
            parentItem = currentItem;
            prevLevel = 0;
        }

    }
    settings()->endArray();

    m_hotkey.clear();
    size = settings()->beginReadArray("HotKeySettings");
    for (int i = 0; i < size; ++i) {
        settings()->setArrayIndex(i);
        QString name = settings()->value("name").toString();
        QKeySequence hotkey = settings()->value("hotkey").value<QKeySequence>();
        m_hotkey.insert(name,hotkey);
    }
    settings()->endArray();

    settings()->endGroup();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
