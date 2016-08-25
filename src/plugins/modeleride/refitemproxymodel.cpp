#include "refitemproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include  <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefItemProxyModel::RefItemProxyModel(QObject *parent):QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setRecursion(true);
}

bool RefItemProxyModel::filterAcceptsRowItself(qint32 source_row, const QModelIndex &source_parent) const
{
    QModelIndex srcIndex = sourceModel()->index(source_row, 0, source_parent);
    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();
    if (tag == DBMODELXML::MODEL || tag == DBREFLISTXML::REFLIST)
        return true;

     if (tag == DBREFGROUPXML::REFGROUP) {
         qint32 row = 0;
         QModelIndex childIndex = srcIndex.child(row,0);
         while (childIndex.isValid())
         {
             if (filterAcceptsRowItself(row, srcIndex))
                 return true;
             childIndex = srcIndex.child(++row,0);
         }
     }

     if (tag == DBREFXML::REF) {
         if (!m_classIndex.isValid())
             return true;

         TreeXmlHashModel *hashModel = qobject_cast<TreeXmlHashModel *>(sourceModel());
         if (hashModel) {
             qint32 refClassColumn = hashModel->columnDisplayedAttr(
                         DBLINKTOCLASSXML::LINKTOCLASS,
                         DBLINKTOCLASSXML::REFCLASS);

             qint32 row = 0;
             QModelIndex childIndex = srcIndex.child(row,0);
             while (childIndex.isValid())
             {
                 if (childIndex.data(TreeXmlModel::TagRole) == DBLINKTOCLASSXML::LINKTOCLASS)
                 {
                     QModelIndex linkIndex = hashModel->indexLink(
                                 childIndex.sibling(childIndex.row(),
                                                    refClassColumn));
                     if (hasAcceptedChildren(linkIndex.sibling(linkIndex.row(),0),
                                             m_classIndex))
                         return true;
                 }
                 childIndex = srcIndex.child(++row, 0);
             }
         }
     }

    return false;
}

bool RefItemProxyModel::recursion() const
{
    return m_recursion;
}

void RefItemProxyModel::setRecursion(bool recursion)
{
    m_recursion = recursion;
}

bool RefItemProxyModel::filterAcceptsRow(qint32 source_row, const QModelIndex &source_parent) const
{
    // Если узел удолетворяет  фильтру то показать этот узел
    if (filterAcceptsRowItself(source_row, source_parent))
           return true;

    return false;
}

bool RefItemProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return QSortFilterProxyModel::lessThan(left,right);
}

void RefItemProxyModel::setClassIndex(QModelIndex &idx)
{
    if (idx.isValid())
        if (idx.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS) {
            m_classIndex = idx.sibling(idx.row(),0);
            return;
        }
    m_classIndex = QModelIndex();
}

QModelIndex RefItemProxyModel::classIndex()
{
    return m_classIndex;
}

bool RefItemProxyModel::hasAcceptedChildren(const QModelIndex &link_index,
                                            const QModelIndex &source_parent) const
{
    if (!source_parent.isValid())
        return false;

    if (source_parent.data(TreeXmlModel::TagRole) != DBCLASSXML::CLASS)
        return false;

    if (source_parent == link_index)
        return true;

    if (recursion()) {
        qint32 row = 0;
        QModelIndex childIndex = sourceModel()->index(row, 0, source_parent);
        while (childIndex.isValid())
        {
            if (hasAcceptedChildren(link_index, childIndex))
                return true;
            childIndex = sourceModel()->index(++row, 0, source_parent);
        }
    }

    return false;
}


}}
