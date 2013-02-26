#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QMenu>

//! Класс ToolBar представляет собой панель инструментов
/*! Панель инструментов состоит из списочных или выпадающих пунктов меню
 *  Отличается от QToolBar возможносnm добавления QAction при помощи
 *  Drag and Drop
 */
class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = 0);
    virtual ~ToolBar();

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

    //! Фильтр событий
    bool eventFilter(QObject *object, QEvent *event);

    //! Обработка событий работы с QAction
    void actionEvent(QActionEvent *event);

    //! Вызов контекстного меню
    void contextMenuEvent(QContextMenuEvent *event);
    
private slots:

    //! Удаление QAction на котором вызвано контекстное меню
    void removeContextAction();

    //! Вызов редактирования свойств QAction
    void showActionProp();

private:

    //! Старые координаты курсора мыши при Drag and Drop
    QPoint m_dragPos;

    //!  Активный QAction с меню
    QAction *m_activeAction;

    //! Активный QAction при вызове контекстного меню
    QAction *m_contextAction;
};

#endif // TOOLBAR_H
