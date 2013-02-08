#ifndef MENU_H
#define MENU_H

#include <QMenu>

class Menu : public QMenu
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = 0);
    explicit Menu(const QString & title, QWidget * parent = 0 );

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

   QSize sizeHint() const;

signals:
    
public slots:
    
};

#endif // MENU_H
