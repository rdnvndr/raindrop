#include "permissionproxymodel.h"

#include <QDebug>

#include <treexmlmodel/treexmlmodel.h>
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PermissionProxyModel::PermissionProxyModel()
{

}

PermissionProxyModel::~PermissionProxyModel()
{

}

void PermissionProxyModel::setRootIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

QModelIndex PermissionProxyModel::rootIndex()
{
    return m_rootIndex;
}

bool PermissionProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex source_index = source_parent.child(source_row,0);
    QString tag =
            sourceModel()->data(source_index, TreeXmlModel::TagRole).toString();

    if (m_rootIndex == source_parent
            && tag != DBATTRXML::ATTR && tag != DBPERMISSIONXML::PERMISSION)
        return false;

    if (sourceModel()->parent(m_rootIndex) == source_parent && source_index != m_rootIndex)
        return false;

    return true;
}

bool PermissionProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant rightData = right.data(TreeXmlModel::TagRole);
    QVariant leftData  = left.data(TreeXmlModel::TagRole);

    if (rightData != DBCLASSXML::CLASS) {
        if (leftData == DBCLASSXML::CLASS)
            return true;
    } else {
        if (leftData == DBCLASSXML::CLASS)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBCOMPXML::COMP) {
        if (leftData == DBCOMPXML::COMP)
            return true;
    } else {
        if (leftData == DBCOMPXML::COMP)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBFILTERXML::FILTER) {
        if (leftData == DBFILTERXML::FILTER )
            return true;
    } else {
        if (leftData == DBFILTERXML::FILTER )
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBPERMISSIONXML::PERMISSION) {
        if (leftData == DBPERMISSIONXML::PERMISSION )
            return true;
    } else {
        if (leftData == DBPERMISSIONXML::PERMISSION )
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    return QSortFilterProxyModel::lessThan(left,right);
}

}}

