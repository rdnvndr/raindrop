#include "exampleplug.h"
#include <QTextEdit>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace Example {

ExamplePlug::ExamplePlug(QObject *parent):
    QObject(parent), IPlugin("IMainWindow ITreeDockWidget")
{
    // Получение интерфейса ITreeDockWidget
    PluginManager *pluginManager = PluginManager::instance();
    ITreeDockWidget *dockWidget =
            pluginManager->interfaceObject<ITreeDockWidget*>("ITreeDockWidget");
    if (dockWidget == nullptr)
        return;

    QTreeWidget *tree = dockWidget->insertTreeWidget(
                QIcon(tr(":/example")),tr("Проверка"));

    QList<QTreeWidgetItem*> items;

    // Создаем узел - раздел в дереве
    QTreeWidgetItem *item = new QTreeWidgetItem();

    // Создаем шрифт
    QFont font = item->font(0);
    font.setBold(true);

    // Устанавливаем шрифт
    item->setFont(0, font);

    // Устанавливаем имя узлу - разделу
    item->setText(0, tr("Примеры"));
    items.append(item);

    // Создаем узел для создания закладки
    QTreeWidgetItem *itemnode = new QTreeWidgetItem(item);
    itemnode->setIcon(0, QIcon(tr(":/example")));
    itemnode->setText(0, tr("Создание закладки"));
    dockWidget->setFuncTreeItem(itemnode, this,"createTab");

    // Создаем узел для создания диалогового окна
    itemnode = new QTreeWidgetItem(item);
    itemnode->setIcon(0, QIcon(tr(":/example")));
    itemnode->setText(0, tr("Создание диалога"));
    dockWidget->setFuncTreeItem(itemnode, this,"createDlgWindow");
    tree->insertTopLevelItems(0,items);
}

void ExamplePlug::createTab(){

    // Получение интерфейса IMainWindow
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *mainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");

    mainWindow->addSubWindow(new QTextEdit(NULL));
}

void ExamplePlug::createDlgWindow(){
    QMessageBox::warning(NULL,tr("Инициализация"),tr("Инициализация плагина Example"));
}

}}
