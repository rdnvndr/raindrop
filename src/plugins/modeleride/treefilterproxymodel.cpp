#include "treefilterproxymodel.h"
#include <QDebug>
#include "dbxmlstruct.h"
#include <QMimeData>
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/mimedataindex.h>

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

bool TreeFilterProxyModel::filterAcceptsRow(int source_row,
                                            const QModelIndex &source_parent) const{

    if (!m_filterTags.contains(sourceModel()->data(
                                  source_parent.child(source_row,0),
                                  Qt::UserRole).toString()) && source_parent.isValid())
            return false;

    // Если узел удолетворяет  фильтру то показать этот узел
    if (filterAcceptsRowItself(source_row, source_parent))
           return true;

    // Если хотя бы 1 ребенок показан по фильтру, то показать и этот узел
    if (hasAcceptedChildren(source_row, source_parent))
        return true;

    return false;
}

bool TreeFilterProxyModel::filterAcceptsRowItself(int source_row,
                                                  const QModelIndex &source_parent) const
{
    if (!m_filterTags.contains(sourceModel()->data(
                                  source_parent.child(source_row,0),
                                  Qt::UserRole).toString()) && source_parent.isValid())
        return false;

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool TreeFilterProxyModel::hasAcceptedChildren(int source_row,
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

bool TreeFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (right.data(Qt::UserRole)!=DBCLASSXML::CLASS) {
        if (left.data(Qt::UserRole)==DBCLASSXML::CLASS)
            return true;
    } else {
        if (left.data(Qt::UserRole)==DBCLASSXML::CLASS)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (right.data(Qt::UserRole)!=DBCOMPXML::COMP) {
        if (left.data(Qt::UserRole)==DBCOMPXML::COMP)
            return true;
    } else {
        if (left.data(Qt::UserRole)==DBCOMPXML::COMP)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (right.data(Qt::UserRole)!=DBFILTERXML::FILTER) {
        if (left.data(Qt::UserRole)==DBFILTERXML::FILTER )
            return true;
    } else {
        if (left.data(Qt::UserRole)==DBFILTERXML::FILTER )
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
