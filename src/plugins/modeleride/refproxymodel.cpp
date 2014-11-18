#include "refproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>

RefProxyModel::RefProxyModel(QObject *parent) :
    ModifyProxyModel(parent)
{
}

QVariant RefProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role == Qt::DecorationRole && proxyIndex.column() != 0)
        return QVariant();

    return ModifyProxyModel::data(proxyIndex, role);
}
