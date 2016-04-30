#include "refitemproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include  <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefItemProxyModel::RefItemProxyModel(QObject *parent)
{
    Q_UNUSED(parent)

    setDynamicSortFilter(true);
}

bool RefItemProxyModel::hasAcceptedChildren(int source_row,
                                               const QModelIndex &source_parent) const
{
    QModelIndex item = sourceModel()->index(source_row,0,source_parent);
    if (!item.isValid())
        return false;


    int childCount = item.model()->rowCount(item);
    if (childCount == 0)
        return false;

    for (int i = 0; i < childCount; ++i) {
        if (filterAcceptsRowItself(i, item))
            return true;
        if (hasAcceptedChildren(i, item))
            return true;
    }
    return false;
}

bool RefItemProxyModel::filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const
{
    TreeXmlHashModel *hashModel = qobject_cast<TreeXmlHashModel *>(sourceModel());
    if (hashModel) {
        QModelIndex srcIndex = hashModel->index(source_row, 0, source_parent);
        QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();

        if (tag == DBMODELXML::MODEL || tag == DBREFLISTXML::REFLIST)
            return true;

        if (tag == DBREFXML::REF) {
            for (QModelIndex srcChild = srcIndex.child(0,0);
                 srcChild.isValid();
                 srcChild = srcChild.sibling(srcChild.row()+1,0))
            {
                if (srcChild.data(TreeXmlModel::TagRole) == DBLINKTOCLASSXML::LINKTOCLASS) {
                    int column = hashModel->columnDisplayedAttr(
                                DBLINKTOCLASSXML::LINKTOCLASS,
                                DBLINKTOCLASSXML::REFCLASS);
                    QString classId = srcChild.sibling(
                                srcChild.row(), column).data(Qt::EditRole).toString();
                    if (m_classId.isEmpty() || classId == m_classId)
                        return true;
                }

            }
            return false;
        }
    }

    return false;
}

bool RefItemProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Если узел удолетворяет  фильтру то показать этот узел
    if (filterAcceptsRowItself(source_row, source_parent))
           return true;

    // Если хотя бы 1 ребенок показан по фильтру, то показать и этот узел
    if (hasAcceptedChildren(source_row, source_parent))
        return true;

    return false;
}

bool RefItemProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return QSortFilterProxyModel::lessThan(left,right);
}

void RefItemProxyModel::setClassId(const QString &classId)
{
    m_classId = classId;
}


}}
