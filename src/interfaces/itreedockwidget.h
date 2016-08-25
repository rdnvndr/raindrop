#ifndef ITREEDOCKWIDGET_H
#define ITREEDOCKWIDGET_H
#include <QDockWidget>
#include <QAction>
#include <QIcon>
#include <QString>
#include <QWidget>
#include <QObject>
#include <QStackedWidget>
#include <QTreeWidgetItem>

//! Класс отображения деревьев с панелей инструментов
/*! Класс отображения деревьев с панелей инструментов
    предназначен для отображения дерева команд для каждого
    модуля отдельно. Переключения модулей осуществляется
    через панель инструментов
*/
class ITreeDockWidget
{

public:

    //! Создание дерева
    /*! Предназначено для создания дерева с указанной иконкой и имененм
        \param icon Иконка дерева
        \param name Имя дерева
    */
    virtual QTreeWidget *insertTreeWidget(QIcon icon,QString name)  = 0;

    //! Установка метода обработки щелчка по узлу узла
    /*! Предназначено для обработки щелчка мыши по узлу
        \param item Узел дерева
        \param obj Объект которому принадлежит метод обработки
        \param funcname Имя функции без параметров и скобок
    */
    virtual void setFuncTreeItem(QTreeWidgetItem *item, QObject *obj, const char *funcname)  = 0;

    //! Вставка визуального компонета
    /*! Предназначено для создания визуального компонента с указанной иконкой и имененм
        \param icon Иконка дерева
        \param name Имя дерева
        \param widget Визуальный компонент
    */
    virtual void insertWidget(QIcon icon, QString name, QWidget *widget)  = 0;
public slots:

    //! Установка активного дерева в зависимости от выбранного модуля
    virtual void setActionTreeWidget()  = 0;

    //! Слот вызова функции узла дерева
    virtual void callFuncTreeWidget(QTreeWidgetItem *item, qint32 column)  = 0;
};

Q_DECLARE_INTERFACE(ITreeDockWidget,"com.RTPTechGroup.Raindrop.ITreeDockWidget/1.0")

#endif 
