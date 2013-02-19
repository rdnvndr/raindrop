#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

//! Класс MenuBar представляет собой строку меню
/*! Строка меню состоит из списочных или выпадающих пунктов меню
 *  Отличается от QMenuBar возможносnm добавления QAction при помощи
 *  Drag and Drop
 */

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:

    //! Конструктор класса
    explicit MenuBar(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~MenuBar();

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

    //! Вызов контекстного меню
    void contextMenuEvent(QContextMenuEvent *event);

private:

    //! Контекстное меню
    QMenu* m_contextMenu;

    //! Старые координаты курсора мыши при Drag and Drop
    QPoint m_dragPos;
};

#endif // MENUBAR_H
