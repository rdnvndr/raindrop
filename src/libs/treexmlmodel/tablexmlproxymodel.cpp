#include "tablexmlproxymodel.h"
#include <QtCore>

TableXMLProxyModel::TableXMLProxyModel()
{
}

bool TableXMLProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    if (mapFromSource(parent) == m_index)
        return QSortFilterProxyModel::filterAcceptsRow(row,parent);
    else
        return true;
}

void TableXMLProxyModel::setFilterIndex(const QModelIndex &parent)
{
    m_index = mapFromSource(parent);
}

QModelIndex TableXMLProxyModel::filterIndex()
{
    return m_index;
}
