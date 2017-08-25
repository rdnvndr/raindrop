#ifndef ACTIONGROUPTREEVIEW_H
#define ACTIONGROUPTREEVIEW_H

#include <QTreeView>

namespace RTPTechGroup {
namespace MainWindow {

//! Дерево команд
/*! Дерево команд отличается от стандартного дерева поддержкой Drag and Drop
 */
class ActionGroupTreeView : public QTreeView
{
    Q_OBJECT
public:
     //! Конструктор класса
    explicit ActionGroupTreeView(QWidget *parent = 0);

    //! Действия для создание перетаскивания
    void startDrag(Qt::DropActions supportedActions);

};

}
}

#endif // ACTIONGROUPTREEVIEW_H
