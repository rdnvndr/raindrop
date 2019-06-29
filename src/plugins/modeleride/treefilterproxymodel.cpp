#include "treefilterproxymodel.h"
#include <metadatamodel/dbxmlstruct.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QMimeData>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

TreeFilterProxyModel::TreeFilterProxyModel()
{
    m_displayFirstColumn = 0;
    m_displaySecondColumn = -1;
    m_formatDisplayColumn = "%1 (%2)";
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

QVariant TreeFilterProxyModel::data(const QModelIndex &index, qint32 role) const
{
    if (role == Qt::DisplayRole && index.column() == 0)
    {
        QString first = QSortFilterProxyModel::data(
                    index.sibling(index.row(), m_displayFirstColumn),
                    role).toString();

        if (m_displaySecondColumn == -1)
            return first;

        QString second = QSortFilterProxyModel::data(
                    index.sibling(index.row(), m_displaySecondColumn),
                    role).toString();

        return QString(m_formatDisplayColumn).arg(first).arg(second);
    }
    return QSortFilterProxyModel::data(index, role);
}

bool TreeFilterProxyModel::filterAcceptsRow(
        qint32 source_row, const QModelIndex &source_parent) const
{
    if (source_parent.isValid()
            && !m_filterTags.contains(sourceModel()->data(
                                          childIdx(source_row,0,source_parent),
                                          TreeXmlModel::TagRole).toString()))
        return false;

    // Если узел удолетворяет  фильтру то показать этот узел
    if (filterAcceptsRowItself(source_row, source_parent))
           return true;

    // Если хотя бы 1 ребенок показан по фильтру, то показать и этот узел
    if (hasAcceptedChildren(source_row, source_parent))
        return true;

    return false;
}

bool TreeFilterProxyModel::filterAcceptsRowItself(
        qint32 source_row, const QModelIndex &source_parent) const
{
    if (source_parent.isValid()
            && !m_filterTags.contains(sourceModel()->data(
                                          childIdx(source_row,0,source_parent),
                                          TreeXmlModel::TagRole).toString()))
        return false;

    if (!this->filterRegExp().isEmpty()) {
        QString first = sourceModel()->data(
                    childIdx(source_row, m_displayFirstColumn, source_parent),
                    Qt::DisplayRole).toString();

        if (m_displaySecondColumn == -1)
            return filterRegExp().indexIn(first) != -1;

        QString second = sourceModel()->data(
                    childIdx(source_row, m_displaySecondColumn, source_parent),
                    Qt::DisplayRole).toString();

        return filterRegExp().indexIn(
                    QString(m_formatDisplayColumn).arg(first).arg(second)) != -1;
    }

    return true;
}

bool TreeFilterProxyModel::hasAcceptedChildren(
        qint32 source_row, const QModelIndex &source_parent) const
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

bool TreeFilterProxyModel::lessThan(
        const QModelIndex &left, const QModelIndex &right) const
{
    QVariant rightData = right.data(TreeXmlModel::TagRole);
    QVariant leftData  = left.data(TreeXmlModel::TagRole);

    if (rightData != DBCLASSLISTXML::CLASSLIST) {
        if (leftData == DBCLASSLISTXML::CLASSLIST)
            return true;
    } else {
        if (leftData == DBCLASSLISTXML::CLASSLIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBLOVLISTXML::LOVLIST) {
        if (leftData == DBLOVLISTXML::LOVLIST)
            return true;
    } else {
        if (leftData == DBLOVLISTXML::LOVLIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBNUMERATORLISTXML::NUMERATORLIST) {
        if (leftData == DBNUMERATORLISTXML::NUMERATORLIST)
            return true;
    } else {
        if (leftData == DBNUMERATORLISTXML::NUMERATORLIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBREFLISTXML::REFLIST) {
        if (leftData == DBREFLISTXML::REFLIST)
            return true;
    } else {
        if (leftData == DBREFLISTXML::REFLIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBROLELISTXML::ROLELIST) {
        if (leftData == DBROLELISTXML::ROLELIST)
            return true;
    } else {
        if (leftData == DBROLELISTXML::ROLELIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }

    if (rightData != DBQUANTITYLISTXML::QUANTITYLIST) {
        if (leftData == DBQUANTITYLISTXML::QUANTITYLIST)
            return true;
    } else {
        if (leftData == DBQUANTITYLISTXML::QUANTITYLIST)
            return QSortFilterProxyModel::lessThan(left,right);
        return false;
    }


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

QModelIndex TreeFilterProxyModel::childIdx(int arow, int acolumn, const QModelIndex &parent) const
{
    return parent.model() ? parent.model()->index(arow, acolumn, parent)
                          : QModelIndex();
}

QString TreeFilterProxyModel::formatDisplayColumn() const
{
    return m_formatDisplayColumn;
}

void TreeFilterProxyModel::setFormatDisplayColumn(const QString &formatDisplayColumn)
{
    m_formatDisplayColumn = formatDisplayColumn;
}

void TreeFilterProxyModel::setDisplayColumn(qint32 first, qint32 second)
{
    m_displayFirstColumn = first;
    m_displaySecondColumn = second;
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

bool TreeFilterProxyModel::dropMimeData(
        const QMimeData *data, Qt::DropAction action,
        qint32 row, qint32 column, const QModelIndex &parent)
{
    Q_UNUSED (row)
    Q_UNUSED (column)
    bool success = QSortFilterProxyModel::dropMimeData(
                data, action, -1, -1, parent);
    this->setFilterKeyColumn(this->filterKeyColumn());
    return success;
}

}}
