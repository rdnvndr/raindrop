#include "exampleplug.h"
#include <mdiextarea/mdiextarea.h>
#include <QTextEdit>

ExamplePlug::ExamplePlug(QObject *parent):
    QObject(parent)
{
    setName(tr("Пример плагина"));
    setIcon(QIcon(":/example"));
    addDepend("ITreeDockWidget");
}

bool ExamplePlug::initialize()
{
    // Получение интерфейса ITreeDockWidget
    PluginManager* pluginManager = PluginManager::instance();
    ITreeDockWidget* dockWidget = qobject_cast<ITreeDockWidget*>(
                pluginManager->getPlugin("ITreeDockWidget")->instance());

    QTreeWidget* tree = dockWidget->insertTreeWidget(
                QIcon(tr(":/example")),tr("Проверка"));

    QList<QTreeWidgetItem*> items;

    // Создаем узел - раздел в дереве
    QTreeWidgetItem* item = new QTreeWidgetItem();

    // Создаем шрифт
    QFont font = item->font(0);
    font.setBold(true);

    // Устанавливаем шрифт
    item->setFont(0, font);

    // Устанавливаем имя узлу - разделу
    item->setText(0, tr("Примеры"));
    items.append(item);

    // Создаем узел для создания закладки
    QTreeWidgetItem* itemnode = new QTreeWidgetItem(item);
    itemnode->setIcon(0, QIcon(tr(":/example")));
    itemnode->setText(0, tr("Создание закладки"));
    dockWidget->setFuncTreeItem(itemnode, this,"createTab");

    // Создаем узел для создания диалогового окна
    itemnode = new QTreeWidgetItem(item);
    itemnode->setIcon(0, QIcon(tr(":/example")));
    itemnode->setText(0, tr("Создание диалога"));
    dockWidget->setFuncTreeItem(itemnode, this,"createDlgWindow");
    tree->insertTopLevelItems(0,items);

    return true;
}

bool ExamplePlug::release()
{
    return true;
}

void ExamplePlug::createTab(){

    // Получение интерфейса IMainWindow
    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->getPlugin("IMainWindow")->instance());

    mainWindow->addSubWindow(new QTextEdit(NULL));
}

void ExamplePlug::createDlgWindow(){
    QMessageBox::warning(NULL,tr("Инициализация"),tr("Инициализация плагина Example"));
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(example, ExamplePlug)
#endif
