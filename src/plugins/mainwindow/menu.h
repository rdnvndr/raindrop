#ifndef MENU_H
#define MENU_H

#include <QMenu>

class Menu : public QMenu
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    explicit Menu(const QString & title, QWidget * parent = 0 );
    virtual ~Menu();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

   QSize sizeHint() const;

   void contextMenuEvent(QContextMenuEvent *event);


private:
   QMenu* m_contextMenu;
   QPoint m_dragPos;
    
};

#endif // MENU_H
