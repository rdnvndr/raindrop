#include "listview.h"
#include "mimedataobject.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QDebug>

ListView::ListView(QWidget *parent) :
    QListView(parent)
{
    setAcceptDrops(true);
}

void ListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    QList<QPersistentModelIndex> persistentIndexes;

    if (indexes.count() > 0) {
        QMimeData *data = model()->mimeData(indexes);
        if (!data)
            return;

        for (int i = 0; i<indexes.count(); i++){
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
            for (int i = 0; i<indexes.count(); i++){
                QPersistentModelIndex idx = persistentIndexes.at(i);
                if (idx.isValid())
                    model()->removeRow(idx.row(), idx.parent());
                else
                    model()->removeRow(idx.row(), QModelIndex());
            }
        }
    }
}
