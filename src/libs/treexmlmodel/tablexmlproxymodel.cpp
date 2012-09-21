#include "tablexmlproxymodel.h"
#include <QtCore>
#include <QDebug>

TableXMLProxyModel::TableXMLProxyModel(): QSortFilterProxyModel()
{
    setDynamicSortFilter(true);
}

bool TableXMLProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{

    if (parent == m_index && parent.isValid()){
        /*QString tag = sourceModel()->data(
                    sourceModel()->index(row,0,parent),
                    Qt::UserRole).toString();
        foreach (QString tagName,m_tags)
            if (tag==tagName)*/
                return QSortFilterProxyModel::filterAcceptsRow(row,parent);
        return false;
    }

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

void TableXMLProxyModel::setAttributeTags(QStringList list)
{
    m_tags = list;
}

QStringList TableXMLProxyModel::attributeTags(QStringList list)
{
    return m_tags;
}
