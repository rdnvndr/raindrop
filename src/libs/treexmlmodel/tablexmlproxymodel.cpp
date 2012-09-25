#include "tablexmlproxymodel.h"
#include <QtCore>
#include <QDebug>

TableXMLProxyModel::TableXMLProxyModel(): QSortFilterProxyModel()
{
    setDynamicSortFilter(true);
}

bool TableXMLProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    QModelIndex source_index = sourceModel()->index(row,filterKeyColumn(),parent);
    if (!source_index.isValid()) // the column may not exist
        return true;

    if (parent.internalPointer() ==  m_index.internalPointer()){
        QString tag = sourceModel()->data(source_index, Qt::UserRole).toString();
        foreach (const QString& tagName,this->m_tags)
            if (tag.contains(tagName)){
                if (filterRegExp() == QRegExp("\\S*")){
                    if (sourceModel()->data(source_index).toString().isEmpty())
                        return true;
                    else
                        return false;
                }
                return QSortFilterProxyModel::filterAcceptsRow(row,parent);
            }
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

QStringList TableXMLProxyModel::attributeTags()
{
    return m_tags;
}
