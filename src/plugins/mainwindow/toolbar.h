#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = 0);

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
    
signals:
    
public slots:

private:
    //! Старые координаты курсора мыши при Drag and Drop
    QPoint m_dragPos;
};

#endif // TOOLBAR_H
