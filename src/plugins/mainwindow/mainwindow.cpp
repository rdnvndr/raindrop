#include <QCloseEvent>
#include <QString>
#include "mainwindow.h"
#include "menubar.h"
#include "menu.h"

#include <plugin/pluginmanager.h>
#include <QUuid>

MainWindow::MainWindow(QMainWindow* pwgt) : QMainWindow(pwgt), IPlugin("")
{

    setupUi(this);
    this->setMenuBar(new MenuBar());
    //writeMenuSettings();

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

    readSettings();
    readMenuSettings();

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

    show();
}

MainWindow::~MainWindow()
{
    writeSettings();
    settings()->sync();

    cleanAction(m_item);
    delete m_item;
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
    resize(settings()->value("size", QSize(687, 582)).toSize());
    move(settings()->value("pos", QPoint(200, 200)).toPoint());
    if (settings()->value("viewMode")== QMdiArea::SubWindowView)
        setWindowModeEnable(true);
    else
        setWindowModeEnable(false);
    settings()->endGroup();
}


void MainWindow::writeMenuSettings() {
    // MenuBar settings
    settings()->beginGroup("IMainWindow");
    m_menuArrayIndex = 0;
    settings()->beginWriteArray("MenuBar");
    writeMenu(this->menuBar());
    settings()->endArray();
    settings()->endGroup();
}

void MainWindow::writeSettings()
{
    // MainWindow settings
    settings()->beginGroup("IMainWindow");
    settings()->setValue("size", size());
    settings()->setValue("pos", pos());
    settings()->setValue("viewMode", mdiArea->viewMode());
    settings()->endGroup();
}

void MainWindow::cleanAction(MenuItem *menuItem)
{
    foreach (MenuItem *item,menuItem->childItems){
        if (item->action)
        {
            if (item->action->isSeparator() || item->action->menu())
                delete item->action;
            item->action = NULL;
        }
        cleanAction(item);
    }
}

QAction *MainWindow::createAction(MenuItem *menuItem)
{
    if (!menuItem)
        return NULL;

    if (menuItem->action && menuItem->type == "Menu")
        return menuItem->action;

    MenuItem *parentItem = menuItem->parentItem;
    if (!parentItem)
        return NULL;

    // Сортировка меню
    QAction *parentAction  = createAction(parentItem);
    QMenu *parentMenu = (parentAction) ? parentAction->menu():NULL;
    QAction *prevAction = NULL;
    MenuItem *separatorItem = NULL;

    for (int row = parentItem->childItems.count()-1; row >= 0; row--) {
        MenuItem *item = parentItem->childItems.at(row);
        if (item == menuItem) {

            // Создание разделителя между двумя существующими QAction
            if (separatorItem) {
                prevAction = (parentMenu) ?
                            parentMenu->insertSeparator(prevAction)
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
        } else {
            if (prevAction)
                menuBar()->insertAction(prevAction,menuItem->action);
            else
                menuBar()->addAction(menuItem->action);
        }
    }

    return NULL;
}

void MainWindow::releaseAction(MenuItem *menuItem)
{
    if (menuItem) {

        if (menuItem->childItems.count()==0) {
            MenuItem *parentMenuItem = menuItem->parentItem;
            if (menuItem->action) {
                if (menuItem->action->menu())
                    delete menuItem->action;
                else if (parentMenuItem)
                    if (parentMenuItem->action)
                        if (parentMenuItem->action->menu())
                            parentMenuItem->action->menu()->removeAction(menuItem->action);
            }

            if (parentMenuItem) {
                parentMenuItem->childItems.removeOne(menuItem);
                releaseAction(parentMenuItem);
            }
            delete menuItem;
        }
    }
}

void MainWindow::addAction(QString category, QAction *action)
{
    m_actions.insert(category,action);

    QString name = action->objectName();

    foreach (MenuItem *menuItem,m_actionItem.values(name)) {
        menuItem->action = action;
        createAction(menuItem);
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
    foreach (MenuItem *item, m_actionItem.values(name)) {
        releaseAction(item);
        m_actionItem.remove(name, item);
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

void MainWindow::refreshMenuBar()
{
    this->setMenuBar(new MenuBar());
    cleanAction(m_item);
    foreach (QAction* action, m_actions.values()) {
        if (action)
            foreach (MenuItem *menuItem, m_actionItem.values(action->objectName())) {
                menuItem->action = action;
                createAction(menuItem);
            }
    }
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

QToolBar *MainWindow::getToolBarMain()
{
    return toolBarMain;
}

MdiExtArea *MainWindow::getMdiArea()
{
    return mdiArea;
}

QMdiSubWindow *MainWindow::showOptionsDialog()
{
    if (m_optionsDialog) {
        m_optionsDialog = new MainWindowOptions(this);
        m_optionsDialog->createActionsModel(&m_actions);

        QMdiSubWindow *subWindow = addSubWindow(m_optionsDialog);
        connect(subWindow,SIGNAL(destroyed()),this,SLOT(refreshMenuBar()));

        connect(m_optionsDialog->pushButtonCancel,SIGNAL(clicked()),
                this,SLOT(cancelOptionsDialog()));
        connect(m_optionsDialog->pushButtonCancel,SIGNAL(clicked()),
                subWindow,SLOT(close()));

        connect(m_optionsDialog->pushButtonSave,SIGNAL(clicked()),
                this,SLOT(saveOptionsDialog()));
        connect(m_optionsDialog->pushButtonSave,SIGNAL(clicked()),
                subWindow,SLOT(close()));

        return subWindow;
    }

    return 0;
}

void MainWindow::saveOptionsDialog()
{
    writeMenuSettings();
    readMenuSettings();
}

void MainWindow::cancelOptionsDialog()
{

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

void MainWindow::readMenuSettings()
{
    m_actionItem.clear();
    settings()->beginGroup("IMainWindow");
    //return;
    int prevLevel = 0;
    m_item = new MenuItem;
    m_item->type = "MenuBar";
    m_item->name = "MenuBar";
    m_item->text = "MenuBar";
    m_item->action = NULL;
    m_item->parentItem = NULL;

    MenuItem *parentItem  = m_item;
    MenuItem *currentItem = new MenuItem;

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
        currentItem->action = NULL;
        currentItem->parentItem = parentItem;
        parentItem->childItems.append(currentItem);
        m_actionItem.insert(name,currentItem);
        prevLevel = level;
    }
    settings()->endArray();
    settings()->endGroup();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(mainwindow, MainWindow)
#endif
