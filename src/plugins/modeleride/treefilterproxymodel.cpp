#include "treefilterproxymodel.h"
#include <QDebug>
#include "dbxmlstruct.h"
#include <QMimeData>
#include <treexmlmodel/treexmlmodel.h>

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
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    TreeXMLModel* xmlModel = qobject_cast<TreeXMLModel*>(sourceModel());

    foreach (const QModelIndex& index,indexes){
        QModelIndex sourceIndex = mapToSource(index);
        if (sourceIndex.isValid())
            if (!xmlModel->isInherited(sourceIndex)){
                stream << QString("^");
                QString tag = xmlModel->data(sourceIndex,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < xmlModel->columnCount(sourceIndex); i++){
                    QString attrName = xmlModel->displayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << sourceIndex.sibling(sourceIndex.row(),i).data(Qt::EditRole);
                    }
                }
                if (!xmlModel->isAttr(sourceIndex))
                    packData(sourceIndex,stream);
            }
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setParent(sourceModel());
    mimeData->setData("application/classxmlmodel", encodedData);
    return mimeData;
}

bool TreeFilterProxyModel::dropMimeData(const QMimeData *data,
                                        Qt::DropAction action,
                                        int row, int column,
                                        const QModelIndex &parent)
{
    if (!parent.isValid())
        return false;

    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/classxmlmodel"))
        return false;
    if (column >= columnCount(parent))
        return false;

    QByteArray encodedData = data->data("application/classxmlmodel");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    return unpackData(mapToSource(parent),stream,row,
                      data->parent() == sourceModel()
                      && action == Qt::MoveAction);
}

bool TreeFilterProxyModel::unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move)
{
    QString tag;
    QModelIndex index,ii;
    TreeXMLModel* xmlModel = qobject_cast<TreeXMLModel*>(sourceModel());
    bool nextTag = false;

    while (!stream.atEnd()) {
        QString nameAttr;
        stream >> nameAttr;
        if (nameAttr==QString("^")){
            stream >> tag;
            xmlModel->setInsTagName(tag);
            if (insertRow(0, mapFromSource(parent)))
                nextTag = false;
            else
                nextTag = true;
            index = xmlModel->lastInsertRow();
        } else if (nameAttr==QString("{")) {
            unpackData(xmlModel->lastInsertRow(),stream,row);
        } else if (nameAttr==QString("}")) {
            return true;
        } else if (!nextTag){
            QVariant value;
            stream >> value;
            int column = xmlModel->columnDisplayedAttr(tag,nameAttr);

            QModelIndex existIndex = xmlModel->indexHashAttr(tag,nameAttr,value);
            if (existIndex.isValid() && move)
                xmlModel->refreshHashingOne(existIndex,true);

            if (!xmlModel->setData(index.sibling(index.row(),column),value)){
                nextTag = true;
                xmlModel->removeRow(index.row(),index.parent());
            }
        }
    }
    return true;
}

void TreeFilterProxyModel::packData(QModelIndex parent, QDataStream &stream) const
{
    bool isFirstNode=true;
    TreeXMLModel* xmlModel = qobject_cast<TreeXMLModel*>(sourceModel());

    for (int row=0;row<xmlModel->rowCount(parent);row++)
    {
        QModelIndex sourceIndex = xmlModel->index(row,0,parent);

        // Разделитель
        if (isFirstNode){
            stream << QString("{");
            isFirstNode = false;
        }

        if (sourceIndex.isValid())
            if (!xmlModel->isInherited(sourceIndex)) {
                stream << QString("^");
                QString tag = xmlModel->data(sourceIndex,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < xmlModel->columnCount(sourceIndex); i++) {
                    QString attrName = xmlModel->displayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << sourceIndex.sibling(sourceIndex.row(),i).data(Qt::EditRole);
                    }
                }
                if (!xmlModel->isAttr(sourceIndex))
                    packData(sourceIndex,stream);
            }
    }

    if (!isFirstNode)
        stream << QString("}");
}
