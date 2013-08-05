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

QMimeData *TreeFilterProxyModel::mimeData(const QModelIndexList &indexes) const
{
    TreeXmlHashModel* xmlModel = qobject_cast<TreeXmlHashModel*>(sourceModel());

    PersistentIndexes persistentIndex;

    foreach (const QModelIndex& index,indexes){
        QModelIndex sourceIndex = mapToSource(index);
        if (sourceIndex.isValid())
            if (!xmlModel->isInherited(sourceIndex))
                persistentIndex.append(QPersistentModelIndex(sourceIndex));
    }

    MimeDataIndex *mimeData = new MimeDataIndex();
    mimeData->setIndexes(persistentIndex);
    return mimeData;
}

bool TreeFilterProxyModel::dropMimeData(const QMimeData *data,
                                        Qt::DropAction action,
                                        int row, int column,
                                        const QModelIndex &parent)
{
    Q_UNUSED(row)
    TreeXmlHashModel* xmlModel = qobject_cast<TreeXmlHashModel*>(sourceModel());

    if (!parent.isValid())
        return false;

    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/classxmlmodel"))
        return false;
    if (column >= columnCount(parent))
        return false;

    if (xmlModel->isAttr(mapToSource(parent)))
        return false;

    const  MimeDataIndex *mimeData
            = qobject_cast<const MimeDataIndex *>(data);
    foreach (const QModelIndex& index, mimeData->indexes()){
        if (index.isValid()) {
            if (!xmlModel->isInherited(index)) {
                if (action == Qt::MoveAction)
                    return moveSourceIndex(index, mapToSource(parent), true, true);
                else
                    return copySourceIndex(index, mapToSource(parent), true);
            }
        }
    }

    return true;
}

bool TreeFilterProxyModel::moveSourceIndex(const QModelIndex &srcIndex,
                             const QModelIndex &destIndex, bool recursively,
                             bool first)
{
    TreeXmlHashModel* xmlModel = qobject_cast<TreeXmlHashModel*>(sourceModel());
    if (xmlModel->isInherited(srcIndex))
        return false;

    QString tag = srcIndex.data(Qt::UserRole).toString();
    xmlModel->setInsTagName(tag);

    if (!insertRow(0,mapFromSource(destIndex)))
        return false;

    QModelIndex index = xmlModel->lastInsertRow();

    int i = 0;
    while (!xmlModel->displayedAttr(tag, i).isEmpty()) {
        QString nameAttr = xmlModel->displayedAttr(tag, i);
        QVariant value = srcIndex.data(Qt::EditRole);

        int column = xmlModel->columnDisplayedAttr(tag,nameAttr);

        QModelIndex existIndex = xmlModel->indexHashAttr(tag,nameAttr,value);
        if (existIndex.isValid())
            xmlModel->refreshHashingOne(existIndex,true);

        xmlModel->setData(index.sibling(index.row(),column),value);
        i++;
    }

    bool success = true;
    if (recursively)
        for (int row = 0; row < srcIndex.model()->rowCount(srcIndex); row++) {
            QModelIndex childIndex = srcIndex.child(row,0);
            if (childIndex.isValid())
                    success = moveSourceIndex(childIndex, index, recursively) && success;
        }

    if (!first)
        if (!xmlModel->removeRow(srcIndex.row(),srcIndex.parent()))
            return false;

    return success;
}

bool TreeFilterProxyModel::copySourceIndex(const QModelIndex &srcIndex,
                             const QModelIndex &destIndex, bool recursively)
{
    TreeXmlHashModel* xmlModel = qobject_cast<TreeXmlHashModel*>(sourceModel());
    if (xmlModel->isInherited(srcIndex))
        return false;

    QString tag = srcIndex.data(Qt::UserRole).toString();
    xmlModel->setInsTagName(tag);

    if (!insertRow(0,mapFromSource(destIndex)))
        return false;

    QModelIndex index = xmlModel->lastInsertRow();

    int i = 0;
    while (!xmlModel->displayedAttr(tag, i).isEmpty()) {
        QString nameAttr = xmlModel->displayedAttr(tag, i);
        QVariant value = srcIndex.data(Qt::EditRole);

        int column = xmlModel->columnDisplayedAttr(tag,nameAttr);
        xmlModel->setData(index.sibling(index.row(),column),value);
        i++;
    }

    bool success = true;
    if (recursively)
        for (int row = 0; row < srcIndex.model()->rowCount(srcIndex); row++) {
            QModelIndex childIndex = srcIndex.child(row,0);
            if (childIndex.isValid())
                    success = copySourceIndex(childIndex, index, recursively) && success;
        }

    return success;
}
