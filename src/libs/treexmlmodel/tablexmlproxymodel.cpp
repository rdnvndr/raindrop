#include "tablexmlproxymodel.h"
#include <QtCore>
#include <QDebug>

TableXMLProxyModel::TableXMLProxyModel()
{
    setDynamicSortFilter(true);
}

bool TableXMLProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    if (parent == m_index)
        return QSortFilterProxyModel::filterAcceptsRow(row,parent);

    return true;
}

void TableXMLProxyModel::setFilterIndex(const QModelIndex &parent)
{
    m_index = parent;
}

QModelIndex TableXMLProxyModel::filterIndex()
{
    return m_index;
}
