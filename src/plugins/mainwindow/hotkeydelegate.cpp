#include "hotkeydelegate.h"

HotKeyDelegate::HotKeyDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void HotKeyDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    QStyleOptionViewItemV3 opt = option;

    if (!index.parent().isValid()) {
        opt.font.setBold(true);
        opt.fontMetrics = QFontMetrics(opt.font);
    }

    QItemDelegate::paint(painter, opt, index);
}

