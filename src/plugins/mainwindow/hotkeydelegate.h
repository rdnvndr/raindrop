#ifndef HOTKEYDELEGATE_H
#define HOTKEYDELEGATE_H

#include <QItemDelegate>

//! Делегат отображения горячих клавиш команд
/*! Делегат отображения горячих клавиш команд предназначен для выделения
    групп команд жирным шрифтом.
*/

class HotKeyDelegate : public QItemDelegate
{
    Q_OBJECT
public:

    explicit HotKeyDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    
};

#endif // HOTKEYDELEGATE_H
