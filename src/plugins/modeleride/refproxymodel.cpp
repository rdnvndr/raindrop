#include "refproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>

namespace RTPTechGroup {
namespace ModelerIde {

RefProxyModel::RefProxyModel(QObject *parent) :
    ModifyProxyModel(parent)
{
}

QVariant RefProxyModel::data(const QModelIndex &proxyIndex, qint32 role) const
{
    if (role == Qt::DecorationRole && proxyIndex.column() != 0)
        return QVariant();

    return ModifyProxyModel::data(proxyIndex, role);
}

}}
