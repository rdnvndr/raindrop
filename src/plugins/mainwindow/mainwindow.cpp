#include <QCloseEvent>
#include <QString>
#include <QWhatsThis>
#include <QUuid>
#include <plugin/pluginmanager.h>

#include "mainwindow.h"
#include "menubar.h"
#include "menu.h"
#include "toolbar.h"
#include "mdisubwindow.h"

namespace RTPTechGroup {
namespace MainWindow {

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt), IPlugin("")
{
    setupUi(this);
    this->setMenuBar(new MenuBar());
    setIconSize(QSize(20,20));

    m_optionsDialog = NULL;

    readBarSettings();
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

    actionWindowClose = new QAction(QIcon(":close"), tr("Закрыть"), this);
    actionWindowClose->setObjectName("actionWindowClose");
    connect(actionWindowClose, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
    addAction(tr("Окно"),actionWindowClose);

    actionWindowCloseAll = new QAction(QIcon(":closeall"), tr("Закрыть все"), this);
    actionWindowCloseAll->setObjectName("actionWindowCloseAll");
    connect(actionWindowCloseAll, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    addAction(tr("Окно"),actionWindowCloseAll);

    actionWindowCascade = new QAction(QIcon(":cascade"), tr("Каскадом"), this);
    actionWindowCascade->setObjectName("actionWindowCascade");
    connect(actionWindowCascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
    addAction(tr("Окно"),actionWindowCascade);

    actionWindowTile = new QAction(QIcon(":tile"), tr("Плиткой"), this);
    actionWindowTile->setObjectName("actionWindowTile");
    connect(actionWindowTile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    addAction(tr("Окно"),actionWindowTile);

    actionWindowNext = new QAction(QIcon(":next"), tr("Следующее"), this);
    actionWindowNext->setObjectName("actionWindowNext");
    connect(actionWindowNext, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    addAction(tr("Окно"),actionWindowNext);

    actionWindowPrev = new QAction(QIcon(":previous"), tr("Предыдущее"), this);
    actionWindowPrev->setObjectName("actionWindowPrev");
    connect(actionWindowPrev, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
    addAction(tr("Окно"),actionWindowPrev);

    actionWindowGui = new QAction(QIcon(":window"), tr("Оконный вид"), this);
    actionWindowGui->setCheckable(true);
    actionWindowGui->setObjectName("actionWindowGui");
    connect(actionWindowGui, SIGNAL(triggered(bool)), this, SLOT(setWindowModeEnable(bool)));
    addAction(tr("Окно"),actionWindowGui);

    actionGuiOptions = new QAction(QIcon(":guioptions"), tr("Оформление..."), this);
    actionGuiOptions->setObjectName("actionGuiOptions");
    connect(actionGuiOptions, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
    addAction(tr("Настройка"),actionGuiOptions);

    actionExit = new QAction(QIcon(":exit"), tr("Выход"), this);
    actionExit->setObjectName("actionExit");
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    addAction(tr("Файл"),actionExit);

    actionAboutQt = new QAction(QIcon(":qt"), tr("О Qt"), this);
    actionAboutQt->setObjectName("actionAboutQt");
    connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
    addAction(tr("Справка"),actionAboutQt);

    actionAbout = new QAction(QIcon(":about"), tr("О программе..."), this);
    actionAbout->setObjectName("actionAbout");
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    addAction(tr("Справка"),actionAbout);

    actionWhatsThis = new QAction(QIcon(":whatsthis"), tr("Что это?"), this);
    actionWhatsThis->setObjectName("actionWhatsThis");
    actionWhatsThis->setShortcut(tr("Shift+F1"));
    connect(actionWhatsThis, SIGNAL(triggered()), this, SLOT(showWhatsThis()));
    addAction(tr("Справка"),actionWhatsThis);

    Menu *newMenu = new Menu("Новое меню");
    newMenu->setIcon(QIcon(":menu"));
    newMenu->menuAction()->setObjectName("actionNewMenu");
    addAction(tr("Новое меню"),newMenu->menuAction());

    QAction *newSeparator = new QAction("Разделитель",this);
    newSeparator->setIcon(QIcon(":separator"));
    newSeparator->setSeparator(true);
    newSeparator->setObjectName("actionNewSeparator");
    addAction(tr("Новое меню"),newSeparator);

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
    delete actionAbout;
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
            connect(this,SIGNAL(iconSizeChanged(QSize)),toolBar,SLOT(setIconSize(QSize)));
            toolBar->setIconSize(this->iconSize());
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


    if (readingBarSettings)
        readBarSettings();
    else
        foreach (MenuItem *item, m_item)
            cleanBranchAction(item);

    QList<ToolBar *> toolBars = this->findChildren<ToolBar *> ();
    foreach (ToolBar* toolBar,toolBars) {
        this->removeToolBar(toolBar);
        delete toolBar;
    }
    this->setMenuBar(new MenuBar());

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

    QMdiSubWindow *subWindow = new MdiSubWindow;
    subWindow->setWidget(widget);
    widget->setParent(subWindow);
    mdiArea->addSubWindow(subWindow);

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

void MainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget)
{
    QMainWindow::addDockWidget(area, dockwidget);
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

void MainWindow::about()
{
    QMessageBox::about(this, tr("О программе"),
              "<h3>"+ qApp->property("app_product").toString()  + " "
                    + qApp->property("app_version").toString()  + "</h3>"
                    + tr("Собран ") + qApp->property("app_revision").toString()
                    + "<br>" +
              "<p>" + qApp->property("app_copyright").toString() +". "
                    + tr("Все права защищены.") +
              "<p>" + qApp->property("app_description").toString() + ".");
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

}
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
