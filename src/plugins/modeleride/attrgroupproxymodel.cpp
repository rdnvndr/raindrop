#include "attrgroupproxymodel.h"
#include <QDebug>

namespace RTPTechGroup {
namespace ModelerIde {

AttrGroupProxyModel::AttrGroupProxyModel(QObject *parent)
:QAbstractListModel(parent)
{
    setUniqueColumn(-1);
    m_model = NULL;
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
    if (index.isValid() && (role == Qt::DisplayRole || role == Qt::EditRole))
        return m_list.at(index.row());
    return QVariant();
}

void AttrGroupProxyModel::setModel(QAbstractItemModel *srcModel)
{
    m_model = srcModel;
    connect(srcModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(reset()));
    connect(srcModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(reset()));
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
    int row = 0;
    int count = m_list.count();

    if (!m_list.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, count-1);
        endRemoveRows();
        m_list.clear();
    }
    QModelIndex childIndex = m_model->index(row, m_uniqueColumn, m_rootIndex);
    QSet<QString> hash;
    while (childIndex.isValid())
    {
        QString nameGroup = childIndex.data().toString();
        if (!nameGroup.isEmpty()) hash.insert(nameGroup);
        childIndex = m_model->index(++row, m_uniqueColumn, m_rootIndex);
    }
    m_list = hash.toList();
    if (!m_list.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, m_list.count()-1);
        endInsertRows();
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

}}
