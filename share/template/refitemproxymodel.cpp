#include "refitemproxymodel.h"
#include "treexmlhashmodel.h"
#include <QFont>

namespace RTPTechGroup {
namespace ModelerIde {

struct PrivateModelIndex
{
    int r, c;
    void *p;
    const QAbstractItemModel *m;
};

RefItemProxyModel::RefItemProxyModel(QObject *parent)
    :QAbstractProxyModel (parent)
{

}



void RefItemProxyModel::sourceDataChanged(const QModelIndex &left,
                                         const QModelIndex &right)
{
    for(int column = left.column(); column <= right.column(); ++column)
        for(int row = left.row(); row <= right.row(); ++row) {
            void *p = left.sibling(row, column).internalPointer();
            QPersistentModelIndex removeIndex(createIndex(row,column,p));
        }

    emit dataChanged(mapFromSource(left), mapFromSource(right));
}

void RefItemProxyModel::sourceRowsRemoved(const QModelIndex &parent,
                                         int start, int end)
{

}

void RefItemProxyModel::sourceRowsAboutToBeRemoved(const QModelIndex &parent,
                                                  int start, int end)
{

}

void RefItemProxyModel::sourceRowsInserted(const QModelIndex &parent, int start, int end)
{
    beginInsertRows(mapFromSource(parent), start, end);
    endInsertRows();
}

int RefItemProxyModel::columnCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    return sourceModel()->columnCount(mapToSource(parent));
}

int RefItemProxyModel::rowCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    int parentRowCount = sourceModel()->rowCount(mapToSource(parent));
    return parentRowCount;
}

QModelIndex RefItemProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    QPersistentModelIndex removeIndex(sourceModel()->index(
                                          row, column, mapToSource(parent)));

    return mapFromSource(removeIndex);
}

QModelIndex RefItemProxyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (!mapToSource(index).isValid())
        return QModelIndex();

    return mapFromSource(mapToSource(index).parent());
}

QModelIndex RefItemProxyModel::mapFromSource(const QModelIndex &index) const
{
    if(index.isValid()) {
        QModelIndex sourceIndex = createIndex(index.row(), index.column(),
                                              index.internalPointer());
        if (sourceIndex.isValid())
            return sourceIndex;
    }

    return QModelIndex();
}

QModelIndex RefItemProxyModel::mapToSource(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QModelIndex sourceIndex;
    PrivateModelIndex *hack = reinterpret_cast<PrivateModelIndex*>(&sourceIndex);

    hack->r = index.row();
    hack->c = index.column();
    hack->p = index.internalPointer();
    hack->m = sourceModel();

    if (!sourceIndex.isValid())
        return QModelIndex();

    if (!sourceIndex.isValid())
        return QModelIndex();

    return sourceIndex;
}

QVariant RefItemProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    return sourceModel()->data(mapToSource(proxyIndex),role);
}

bool RefItemProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)

    return false;
}

bool RefItemProxyModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent);
}

void RefItemProxyModel::setSourceModel(QAbstractItemModel *srcModel)
{
    QAbstractProxyModel::setSourceModel(srcModel);

    connect(srcModel, &QAbstractItemModel::rowsRemoved,
            this, &RefItemProxyModel::sourceRowsRemoved);
    connect(srcModel, &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &RefItemProxyModel::sourceRowsAboutToBeRemoved);
    connect(srcModel, &QAbstractItemModel::rowsInserted,
            this, &RefItemProxyModel::sourceRowsInserted);
    connect(srcModel, &QAbstractItemModel::dataChanged,
            this, &RefItemProxyModel::sourceDataChanged);
}

Qt::ItemFlags RefItemProxyModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant RefItemProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{    
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (m_header[section].isNull())
            return sourceModel()->headerData(section, orientation, role);
        else
            return  m_header[section];
    }
    return  QVariant();

}

bool RefItemProxyModel::setHeaderData(int section, Qt::Orientation orientation,
                                     const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

bool RefItemProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

bool RefItemProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return false;
}

QModelIndex RefItemProxyModel::buddy(const QModelIndex &index) const
{
    return QAbstractItemModel::buddy(index);
}

bool RefItemProxyModel::canFetchMore(const QModelIndex &parent) const
{
    return QAbstractItemModel::canFetchMore(parent);
}

}}
