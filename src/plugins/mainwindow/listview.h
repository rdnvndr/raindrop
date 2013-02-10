#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit ListView(QWidget *parent = 0);

    //! Действия для создание перетаскивания
    void startDrag(Qt::DropActions supportedActions);
    
};

#endif // LISTVIEW_H
