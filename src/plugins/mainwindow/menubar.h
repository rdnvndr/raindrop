#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

namespace RTPTechGroup {
namespace MainWindow {

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

    //! Перевод меню в режим редактирования
    void setEdited(bool edited);

    //! Возращает режим редактирования
    bool isEdited();

private slots:

    //! Удаление QAction на котором вызвано контекстное меню
    void removeContextAction();

    //! Вызов редактирования свойств QAction
    void showActionProp();

private:

    //! Старые координаты курсора мыши при Drag and Drop
    QPoint m_dragPos;

    //! Активный QAction при вызове контекстного меню
    QAction *m_contextAction;

    //! Определяет является ли меню редактируемым
    bool m_edited;
};

}
}

#endif // MENUBAR_H
