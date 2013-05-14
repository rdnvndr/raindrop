#include "conditionproxymodel.h"

ConditionProxyModel::ConditionProxyModel(QObject *parent) :
    QAbstractProxyModel(parent)
{
}
int ConditionProxyModel::columnCount(const QModelIndex& parent) const
{
     return sourceModel()->columnCount(mapToSource(parent)) ;
}

int ConditionProxyModel::rowCount(const QModelIndex& parent) const
{
    return sourceModel()->rowCount(mapToSource(parent)) ;
}

QModelIndex ConditionProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return mapFromSource(sourceModel()->index(row, column, mapToSource(parent))) ;
}

QModelIndex ConditionProxyModel::parent(const QModelIndex& index) const
{
    if (index.isValid() == false)
        return QModelIndex();

    return mapFromSource(sourceModel()->parent(mapToSource(index))) ;
}

QModelIndex ConditionProxyModel::mapFromSource(const QModelIndex& index) const
{
    if (index.isValid())
        return createIndex(index.row(), index.column());
    else
        return QModelIndex();
}

QModelIndex ConditionProxyModel::mapToSource(const QModelIndex& index) const
{
    if (index.isValid())
        return sourceModel()->index(index.row(), index.column(), mapToSource(index.parent())) ;
    else
        return QModelIndex();
}
