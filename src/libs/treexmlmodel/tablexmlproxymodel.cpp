#include "tablexmlproxymodel.h"
#include "treexmlmodel.h"
#include <QtCore>
#include <QDebug>

TableXMLProxyModel::TableXMLProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    m_columnCount = 0;
}

bool TableXMLProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    QModelIndex source_index = sourceModel()->index(row,filterKeyColumn(),parent);
    if (!source_index.isValid())
        return true;

    if (parent.internalPointer() ==  m_index.internalPointer()){
        QString tag = sourceModel()->data(source_index, TreeXmlModel::TagRole).toString();
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

void TableXMLProxyModel::setColumnCount(int column)
{
    m_columnCount = column;
}

QStringList TableXMLProxyModel::attributeTags()
{
    return m_tags;
}


int TableXMLProxyModel::columnCount(const QModelIndex &parent) const
{
    if (QSortFilterProxyModel::columnCount(parent)<m_columnCount || m_columnCount<=0)
        return QSortFilterProxyModel::columnCount(parent);
    else
        return m_columnCount;
}

QVariant TableXMLProxyModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (m_header[section].isNull())
            return QSortFilterProxyModel::headerData(section,orientation,role);
        else
            return  m_header[section];
    }
    return  QVariant();
}

bool TableXMLProxyModel::setHeaderData(int section, Qt::Orientation orientation,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;

}
