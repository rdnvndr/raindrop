#include "actiongrouptreeview.h"
#include <QDrag>

namespace RTPTechGroup {
namespace MainWindow {

ActionGroupTreeView::ActionGroupTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setAcceptDrops(true);
}

void ActionGroupTreeView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    QList<QPersistentModelIndex> persistentIndexes;

    if (!indexes.isEmpty()) {
        QMimeData *data = model()->mimeData(indexes);
        if (!data)
            return;

        for (qint32 i = indexes.count() - 1; i >= 0; --i){
            QModelIndex idx = indexes.at(i);
            persistentIndexes.append(QPersistentModelIndex(idx));
        }

        QDrag *drag = new QDrag(this);
/*
        QPixmap pixmap = indexes.first().data(Qt::DecorationRole).value<QPixmap>();
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
*/
        drag->setMimeData(data);

        Qt::DropAction defaultDropAction = Qt::IgnoreAction;
        if (supportedActions & Qt::MoveAction && dragDropMode() != QAbstractItemView::InternalMove)
            defaultDropAction = Qt::MoveAction;

        if (drag->exec(supportedActions, defaultDropAction) == Qt::MoveAction){
            for (qint32 i = indexes.count() - 1; i >= 0; --i){
                QPersistentModelIndex idx = persistentIndexes.at(i);
                model()->removeRow(idx.row(),
                                   idx.isValid() ? idx.parent() : QModelIndex());
            }
        }
    }
}

}
}
