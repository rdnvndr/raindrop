#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H
#include "treecomboboxglobal.h"
#include <QtGui>

class TREECOMBOBOXLIB TreeComboBox : public QComboBox
{
public:
    TreeComboBox(QWidget* parent=0);
    ~TreeComboBox();
    bool eventFilter(QObject*object,QEvent*event);
    virtual void showPopup();
    virtual void hidePopup();

    void setModel(QAbstractItemModel * model);
    void setCurrentModelIndex (const QModelIndex & index);
private:
    bool skipNextHide;
    QTreeView* treeView;

};

#endif // TREECOMBOBOX_H
