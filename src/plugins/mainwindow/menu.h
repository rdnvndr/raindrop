#ifndef MENU_H
#define MENU_H

#include <QMenu>

//! Класс Menu предназначен для создания меню
/*! Класс Menu представляет собой widget для изпользования в строке меню,
 *  контекстном меню и других выпадающих меню. Отличается от QMenu возможносnm
 *  добавления QAction при помощи Drag and Drop.
*/

class Menu : public QMenu
{
    Q_OBJECT

public:

    //! Конструктор класса
    explicit Menu(QWidget *parent = 0);

    //! Конструктор класса
    explicit Menu(const QString & title, QWidget * parent = 0 );

    //! Деструктор класса
    virtual ~Menu();

     //! Событие обрабатывающее перещение курсора мыши
    void mouseMoveEvent(QMouseEvent *event);

    //! Событие обрабатывающее нажатие клавиш мыши
    void mousePressEvent(QMouseEvent *event);

     //! Событие обрабатывающее отпускание перемещаемого объекта
    void dropEvent(QDropEvent *event);

    //! Событие обрабатывающее нажатие мыши при Drag and Drop
    void dragEnterEvent(QDragEnterEvent *event);

    //! Событие обрабатывающее перещение курсора мыши при Drag and Drop
    void dragMoveEvent(QDragMoveEvent *event);

    //! Рекомендованный размер widget
    QSize sizeHint() const;

private slots:

    //! Удаление QAction на котором вызвано контекстное меню
    void removeContextAction();

    //! Вызов редактирования свойств QAction
    void showActionProp();

private:

    //! Контекстное меню
    QMenu* m_contextMenu;

    //! Старые координаты курсора мыши при Drag and Drop
    QPoint m_dragPos;

    //! Активный QAction при вызове контекстного меню
    QAction *m_contextAction;
    
};

#endif // MENU_H
