#include "attrgroupproxymodel.h"
#include <QDebug>

AttrGroupProxyModel::AttrGroupProxyModel(QObject *parent)
:QAbstractListModel(parent)
{
    setUniqueColumn(-1);
}

AttrGroupProxyModel::~AttrGroupProxyModel()
{

}


int AttrGroupProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_list.count();
}

QVariant AttrGroupProxyModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
        return m_list.at(index.row());
    return QVariant();
}

void AttrGroupProxyModel::setSourceModel(QAbstractItemModel *srcModel)
{
    m_model = srcModel;
}

QAbstractItemModel *AttrGroupProxyModel::sourceModel() const
{
    return m_model;
}

void AttrGroupProxyModel::setRootModelIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

QModelIndex AttrGroupProxyModel::rootModelIndex()
{
    return m_rootIndex;
}

void AttrGroupProxyModel::reset()
{
    m_list.clear();
    int row = 0;
    QModelIndex childIndex = m_model->index(row, m_uniqueColumn, m_rootIndex);
    while (childIndex.isValid())
    {
        m_list << childIndex.data().toString();
        childIndex = m_model->index(++row, m_uniqueColumn, m_rootIndex);
    }
}

void AttrGroupProxyModel::setUniqueColumn(int column)
{
    m_uniqueColumn = column;
}

int AttrGroupProxyModel::uniqueColumn() const
{
    return m_uniqueColumn;
}
