#include "treefilterproxymodel.h"
#include <QDebug>
#include "dbxmlstruct.h"

TreeFilterProxyModel::TreeFilterProxyModel()
{

}

void TreeFilterProxyModel::addHiddenTag(QString tag)
{
    m_filterTags.insert(tag);
}

void TreeFilterProxyModel::removeHiddenTag(QString tag)
{
    m_filterTags.remove(tag);
}

QSet<QString> TreeFilterProxyModel::hiddenTags()
{
    return m_filterTags;
}

bool TreeFilterProxyModel::filterAcceptsRow(int source_row,
                                            const QModelIndex &source_parent) const{

    if (m_filterTags.contains(sourceModel()->data(
                                  source_parent.child(source_row,0),
                                  Qt::UserRole).toString()))
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
    if (right.data(Qt::UserRole)!=DBCLASSXML::CLASS &&
            left.data(Qt::UserRole)==DBCLASSXML::CLASS)
            return true;

    if (right.data(Qt::UserRole)!=DBCOMPXML::COMP &&
            right.data(Qt::UserRole)!=DBCLASSXML::CLASS &&
            left.data(Qt::UserRole)==DBCOMPXML::COMP )
            return true;

    return QSortFilterProxyModel::lessThan(left,right);
}
