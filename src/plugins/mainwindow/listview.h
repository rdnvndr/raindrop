#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

//! Список команд
//! Список команд отличается от стандартного поддержкой Drag and Drop

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
