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

    virtual void paintEvent(QPaintEvent *);
    void setDisplayText(QString text);
    QString getDisplayText() const;

private:
    bool skipNextHide;
    QTreeView* treeView;
    QString m_displayText;
};

#endif // TREECOMBOBOX_H
