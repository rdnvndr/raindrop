#ifndef CLASSTREEVIEW_H
#define CLASSTREEVIEW_H

#include "ui_classtreeview.h"
#include "treefilterproxymodel.h"
#include <QAbstractItemModel>
#include <QMenu>

//! Класс для отображения дерева классов
/*! Визуальный компонент предназначен для отображения дерева классов. Включает в
 * себя фильтрацию и сортировку элементов дерева класса.
*/

class ClassTreeView : public QWidget, private Ui::ClassTreeView
{
    Q_OBJECT
    
public:
    //! Конструктор дерева классов
    explicit ClassTreeView(QWidget *parent = 0);

    //! Деструктор дерева классов
    virtual ~ClassTreeView();

    //! Устанавливает модель для дерева классов
    void setModel(QAbstractItemModel *model);

    //! Возращает текущую позицию в дереве классов
    QModelIndex currentIndex() const;

public slots:

    //! Установка текущейпозиции в дереве классов
    void setCurrentIndex(const QModelIndex & index);

    //! Слот вызова отображения контекстного меню дерева классов
    void showContextMenu(const QPoint &point);

    //! Установка отображения атрибутов в дереве классов
    void setShowAttr(bool shown);

    //! Установка отображения состава в дереве классов
    void setShowComp(bool shown);

    //! Установка отображения фильтров в дереве классов
    void setShowFilter(bool shown);

    //! Установка отображения ЕИ в дереве классов
    void setShowUnit(bool shown);

private slots:

    //! Обработка двойного щелчка мыши в дереве классов
    void treeDoubleClicked(const QModelIndex &index);

    //! Удаление модели данных для дерева класса
    void destroyModel();

    //! Вызов команды вставки строки
    void actionInsert();

    //! Вызов команды удаления строки
    void actionRemove();

signals:

    //! Сигнал о двойном щелчке в дереве классов
    void doubleClicked(const QModelIndex &index);

    //! Сигнал вызова команды вставки строки
    void actionInserted();

    //! Сигнал вызова команды удаления строки
    void actionRemoved();

private:
    //! Прокси модель дерева классов. Отвечает за сортировку и фильтрацию дерева классов
    TreeFilterProxyModel* classFilterModel;

    //! Контекстное меню дерева классов
    QMenu*   contextMenu;

    //! Команда добавления класса
    QAction* actionAddClass;

    //! Команда удаления класса
    QAction* actionRemoveClass;

    //! Команда отображения атрибутов в дереве классов
    QAction* actionShowAttr;

    //! Команда отображения состава в дереве классов
    QAction* actionShowComp;

    //! Команда отображения фильтров в дереве классов
    QAction* actionShowFilter;

    //! Команда отображения ЕИ в дереве классов
    QAction* actionShowUnit;

    //! Команда разделитель
    QAction* actionSeparator;
};

#endif // CLASSTREEVIEW_H
