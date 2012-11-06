#ifndef TREECOMMANDSPLUG_H
#define TREECOMMANDSPLUG_H

#include <plugin/iplugin.h>
#include <treedockwidget.h>

//! Плагин дерева команд
/*! Плагин дерева команд представляет из себя плавающее
    окно с деревом. Подпункты дерева являются команды на
    которые ввешаются обработчики для выполнения различных
    действий. \n
    Пример:
    \code
        PluginManager* pluginManager = PluginManager::instance();
        TreeDockWidget* dockWidget = qobject_cast<TreeDockWidget*>(pluginManager->getObjectByName(
                                                   "TreeCommandsPlug::TreeDockWidget"));

        QTreeWidget* tree = dockWidget->insertTreeWidget(QIcon(":/example"),tr("Проверка"));

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
        itemnode->setIcon(0, QIcon(":/example"));
        itemnode->setText(0, tr("Создание закладки"));
        dockWidget->setFuncTreeItem(itemnode, this,"createTab");
    \endcode
*/

class  TreeCommandsPlug: public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

public:
    //! Конструктор плагина Дерево команд
    TreeCommandsPlug(QObject* parent = 0);

    //! Инициализация плагина Дерево команд
    bool initialize();

     //! Освобождение плагина Дерево команд
    bool release();

    //! Чтение и применение настроек плагина Дерево команд
    void readSettings();

    //! Запись настроек плагина Дерево команд
    void writeSettings();
};

#endif
