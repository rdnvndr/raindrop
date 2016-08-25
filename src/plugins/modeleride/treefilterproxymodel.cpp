#include "treefilterproxymodel.h"
#include <metadatamodel/dbxmlstruct.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/mimedataindex.h>
#include <QMimeData>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

TreeFilterProxyModel::TreeFilterProxyModel()
{

}

void TreeFilterProxyModel::addVisibleTag(const QString &tag)
{
    m_filterTags.insert(tag);
}

void TreeFilterProxyModel::removeVisibleTag(const QString &tag)
{
    m_filterTags.remove(tag);
}

QSet<QString> TreeFilterProxyModel::visibleTags()
{
    return m_filterTags;
}

bool TreeFilterProxyModel::filterAcceptsRow(qint32 source_row,
                                            const QModelIndex &source_parent) const{

    if (!m_filterTags.contains(sourceModel()->data(
                                  source_parent.child(source_row,0),
                                  TreeXmlModel::TagRole).toString()) && source_parent.isValid())
            return false;

    // Если узел удолетворяет  фильтру то показать этот узел
    if (filterAcceptsRowItself(source_row, source_parent))
           return true;

    // Если хотя бы 1 ребенок показан по фильтру, то показать и этот узел
    if (hasAcceptedChildren(source_row, source_parent))
        return true;

    return false;
}

bool TreeFilterProxyModel::filterAcceptsRowItself(qint32 source_row,
                                                  const QModelIndex &source_parent) const
{
    if (!m_filterTags.contains(sourceModel()->data(
                                  source_parent.child(source_row,0),
                                  TreeXmlModel::TagRole).toString()) && source_parent.isValid())
        return false;

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool TreeFilterProxyModel::hasAcceptedChildren(qint32 source_row,
                                               const QModelIndex &source_parent) const
{
    QModelIndex item = sourceModel()->index(source_row,0,source_parent);
    if (!item.isValid())
        return false;


    qint32 childCount = item.model()->rowCount(item);
    if (childCount == 0)
        return false;

    for (qint32 i = 0; i < childCount; ++i) {
        if (filterAcceptsRowItself(i, item))
            return true;
        if (hasAcceptedChildren(i, item))
            return true;
    }
    return false;
}

bool TreeFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
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

    return QSortFilterProxyModel::lessThan(left,right);
}


QStringList TreeFilterProxyModel::mimeTypes() const
{
    QStringList types;
    types << "application/classxmlmodel";
    return types;
}

Qt::DropActions TreeFilterProxyModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TreeFilterProxyModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool TreeFilterProxyModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                        qint32 row, qint32 column, const QModelIndex &parent)
{
    Q_UNUSED (row)
    Q_UNUSED (column)
    bool success = QSortFilterProxyModel::dropMimeData(data,action,-1,-1,parent);
    this->setFilterKeyColumn(this->filterKeyColumn());
    return success;
}

}}
