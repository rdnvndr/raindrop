#ifndef TREEDOCKWIDGET_H
#define TREEDOCKWIDGET_H

#include <QDockWidget>
#include <QToolBar>
#include <QStackedWidget>
#include "treefilterwidget.h"
#include "treedockwidgetglobal.h"
#include <itreedockwidget.h>
#include <plugin/iplugin.h>

namespace RTPTechGroup {
namespace TreeDockWidget {

//! Класс отображения деревьев с панелей инструментов
/*! Класс отображения деревьев с панелей инструментов
    предназначен для отображения дерева команд для каждого
    модуля отдельно. Переключения модулей осуществляется
    через панель инструментов
*/
class TREEDOCKWIDGETLIB TreeDockWidget :
        public QDockWidget,
        public IPlugin,
        public ITreeDockWidget
{
    Q_OBJECT
    Q_INTERFACES(IPlugin ITreeDockWidget)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.TreeDockWidget" FILE "treedockwidget.json")
#endif

public:
// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon();}

    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);};

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);};

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return APP_VERSION;};

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);};

// ITreeDockWidget

    //! Конструктор класса отображения деревьев с панелей инструментов
    explicit TreeDockWidget(QWidget *parent = 0);

    //! Создание дерева
    /*! Предназначено для создания дерева с указанной иконкой и имененм
        \param icon Иконка дерева
        \param name Имя дерева
    */
    QTreeWidget* insertTreeWidget(QIcon icon,QString name);

    //! Установка метода обработки щелчка по узлу узла
    /*! Предназначено для обработки щелчка мыши по узлу
        \param item Узел дерева
        \param obj Объект которому принадлежит метод обработки
        \param funcname Имя функции без параметров и скобок
    */
    void setFuncTreeItem(QTreeWidgetItem* item, QObject* obj, const char* funcname);

    //! Вставка визуального компонета
    /*! Предназначено для создания визуального компонента с указанной иконкой и имененм
        \param icon Иконка дерева
        \param name Имя дерева
        \param widget Визуальный компонент
    */
    void insertWidget(QIcon icon, QString name, QWidget *widget);

public slots:

    //! Установка активного дерева в зависимости от выбранного модуля
    void setActionTreeWidget();

    //! Слот вызова функции узла дерева
    void callFuncTreeWidget(QTreeWidgetItem* item, int column);
    
private:
    //! Связь между кнопкой модуля панели инструментов и деревом
    QHash<QAction*, QWidget*> stackedTree;

    //! Связь между узлом дерева и  именем функции
    QHash<QTreeWidgetItem*, const char*> funcTreeItem;

    //! Связь между узлом дерева и объектом с имененм функции
    QHash<QTreeWidgetItem*, QObject*> objTreeItem;

    //! Контейнер деревьев
    QStackedWidget* stackedWidget;

    //! Панель инструмента модулей
    QToolBar* toolbar;
};

}}

#endif // DOCKCOMMANDSWIDGET_H
