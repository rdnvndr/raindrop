#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:

    explicit MenuBar(QWidget *parent = 0);
    virtual ~MenuBar();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

private:

    QMenu* m_contextMenu;
    QPoint m_dragPos;
};

#endif // MENUBAR_H
