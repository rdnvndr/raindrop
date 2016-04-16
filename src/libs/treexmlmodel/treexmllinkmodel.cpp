#include "treexmllinkmodel.h"

namespace RTPTechGroup {
namespace XmlModel {

struct PrivateModelIndex
{
    int r, c;
    void *p;
    const QAbstractItemModel *m;
};


TreeXmlLinkModel::TreeXmlLinkModel(QObject *parent)
    :QAbstractProxyModel (parent)
{

}

int TreeXmlLinkModel::columnCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    return sourceModel()->columnCount(mapToSource(parent));
}

int TreeXmlLinkModel::rowCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    return sourceModel()->rowCount(mapToSource(parent));
}

bool TreeXmlLinkModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent);
}

QModelIndex TreeXmlLinkModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    return mapFromSource(sourceModel()->index(row, column, mapToSource(parent)));
}

QModelIndex TreeXmlLinkModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (!mapToSource(index).isValid())
        return QModelIndex();
    return mapFromSource(mapToSource(index).parent());
}

QModelIndex TreeXmlLinkModel::mapFromSource(const QModelIndex &index) const
{
    if(index.isValid()) {


        QModelIndex sourceIndex = createIndex(index.row(),
                                              index.column(),
                                              index.internalPointer());
        if (sourceIndex.isValid())
            return sourceIndex;
    }

    return QModelIndex();
}

QModelIndex TreeXmlLinkModel::mapToSource(const QModelIndex &index) const
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

    return sourceIndex;
}

QVariant TreeXmlLinkModel::data(const QModelIndex &proxyIndex, int role) const
{
    return sourceModel()->data(mapToSource(proxyIndex),role);
}

bool TreeXmlLinkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return sourceModel()->setData(mapToSource(index),value, role);
}

void TreeXmlLinkModel::setSourceModel(QAbstractItemModel *srcModel)
{
    QAbstractProxyModel::setSourceModel(srcModel);
}

Qt::ItemFlags TreeXmlLinkModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool TreeXmlLinkModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)

    return false;
}

bool TreeXmlLinkModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)

    return false;
}

bool TreeXmlLinkModel::canFetchMore(const QModelIndex &parent) const
{
    return QAbstractItemModel::canFetchMore(parent);
}

}}

