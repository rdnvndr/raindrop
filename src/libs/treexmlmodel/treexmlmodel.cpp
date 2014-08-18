#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "mimedataindex.h"
#include "tagxmlitem.h"
#include "treexmlmodel.h"

TreeXmlModel::TreeXmlModel(QDomNode document, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TagXmlItem(document, NULL);
    m_column = 1;
    m_insTag = "element";
}

TreeXmlModel::~TreeXmlModel()
{
    delete m_rootItem;
}

int TreeXmlModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return  m_column;
}

void TreeXmlModel::addTagFilter(const QString &tag)
{
    m_filterTags << tag;
}

QStringList TreeXmlModel::tagsFilter()
{
    return m_filterTags;
}

void TreeXmlModel::clearTagsFilter()
{
    m_filterTags.clear();
}

void TreeXmlModel::addAttrTag(const QString &tag)
{
    m_attrTags << tag;
}

void TreeXmlModel::clearAttrTags(){
    m_attrTags.clear();
}

bool TreeXmlModel::isInherited(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;
    TagXmlItem *item = toItem(index);
    return item->isInherited();
}

bool TreeXmlModel::moveIndex(const QModelIndex &srcIndex,
                             const QModelIndex &destIndex, bool recursively,
                             bool first)
{
    if (isInherited(srcIndex))
        return false;

    if (!copyIndex(srcIndex, destIndex, recursively))
        return false;

    if (!first)
        if (!removeRow(srcIndex.row(),srcIndex.parent()))
            return false;

    return true;
}

bool TreeXmlModel::copyIndex(const QModelIndex &srcIndex,
                             const QModelIndex &destIndex, bool recursively)
{
    if (isInherited(srcIndex))
        return false;

    QString tag = srcIndex.data(Qt::UserRole).toString();
    setInsTagName(tag);

    QModelIndex index = insertLastRows(0,1,destIndex);
    if (!index.isValid())
        return false;

    int i = 0;
    while (!displayedAttr(tag, i).isEmpty()) {
        QString nameAttr = displayedAttr(tag, i);

        int column = columnDisplayedAttr(tag,nameAttr);
        QVariant value = srcIndex.sibling(srcIndex.row(),column).data(Qt::EditRole);
        setData(index.sibling(index.row(),column),value);
        i++;
    }

    bool success = true;
    if (recursively)
        for (int row = 0; row < srcIndex.model()->rowCount(srcIndex); row++) {
            QModelIndex childIndex = srcIndex.child(row,0);
            if (childIndex.isValid())
                    success = copyIndex(childIndex, index, recursively) && success;
        }

    return success;
}

bool TreeXmlModel::isAttr(const QModelIndex &index) const
{
    TagXmlItem *item = toItem(index);
    QString nodeName = item->nodeName();
    if (m_attrTags.contains(nodeName))
        return true;

    return false;
}

bool TreeXmlModel::isInsert(const QModelIndex &index) const
{
    TagXmlItem *item = toItem(index);

    if (index.isValid()){
        if (m_insertTags[item->nodeName()].contains(m_insTag))
            return true;
        return false;
    } else {
        if (rowCount(index) == 0)
            return !isAttr(index);
        else
            QMessageBox::warning(NULL,
                                 tr("Предупреждение"),
                                 tr("У дерева может быть только один корневой узел"));
    }
    return false;
}

bool TreeXmlModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return true;
}

void TreeXmlModel::addDisplayedAttr(const QString &tag, const QStringList &value, QIcon icon)
{
    if (value.count()>m_column)
        m_column = value.count();
    m_displayedAttr[tag] = value;

    if (!icon.isNull())
        m_displayedIcon[tag] = icon;
}

void TreeXmlModel::addInsertTags(const QString &tag, const QStringList &value)
{
    m_insertTags.insert(tag,value);
}

int TreeXmlModel::columnDisplayedAttr(const QString &tag, const QString &attr) const
{
    for (int i=0;i<m_displayedAttr[tag].count();i++){
        if (m_displayedAttr[tag].at(i)==attr)
            return i;
    }
    return -1;
}

QString TreeXmlModel::displayedAttr(const QString &tag, int column) const
{
    if (m_displayedAttr[tag].count()<=column || column < 0)
        return QString("");
    return m_displayedAttr[tag].at(column);
}

void TreeXmlModel::removeDisplayedAttr(const QString &tag)
{
    bool flagcolumn = false;
    if (m_displayedAttr[tag].count() == m_column)
        flagcolumn = true;

    m_displayedAttr.remove(tag);
    m_displayedIcon.remove(tag);

    if (flagcolumn){
        m_column = 1;
        foreach (QStringList diplayedAttr, m_displayedAttr.values())
            if (diplayedAttr.count()>m_column)
                m_column = diplayedAttr.count();
    }
}

QVariant TreeXmlModel::data(const QModelIndex &index, int role) const
{
    TagXmlItem *item = toItem(index);
    QString tag = item->nodeName();

    if (role == Qt::DecorationRole)
        if (!m_displayedIcon[tag].isNull()&&index.column()==0)
            return m_displayedIcon[tag];

    if (role == Qt::UserRole){
        return tag;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        if (m_displayedAttr[tag].isEmpty())
            return tag;

        if (index.column()<m_displayedAttr[tag].count()){
            QString attrName = displayedAttr(tag,index.column());
            return item->value(attrName);
        }
    }
    return QVariant();
}

void TreeXmlModel::updateModifyRow(int emptyRowAttr, const QModelIndex &parent)
{
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttr(index)){
            updateModifyRow(emptyRowAttr,index);
            int row = this->rowCount(index)-emptyRowAttr-1;
            QModelIndex updateIndex = index.child(row,0);
            emit dataChanged(updateIndex,updateIndex.sibling(
                                 updateIndex.row(), columnCount(updateIndex)));
        }
    }
}

bool TreeXmlModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (role != Qt::EditRole) return false;

    TagXmlItem *item = toItem(index);

    if (index.column()>=m_displayedAttr[item->nodeName()].count())
        return false;

    QString attrName = displayedAttr(item->nodeName(),index.column());

    if (attrName.isEmpty())
        return false;

    QString dataValue = value.toString();
    if (attrName == QString("parent"))
        dataValue = data(index,Qt::EditRole).toString();

    item->setValue(attrName,dataValue);
    emit dataChanged(index,index);

    if (isAttr(index)){
        int emptyRowAttr = 0;
        QModelIndex parent = index.parent();
        for (int i=index.row()+1;i<this->rowCount(parent);i++){
            QModelIndex idx = parent.child(i,parent.column());
            if (isAttr(idx))
                emptyRowAttr++;
        }
        updateModifyRow(emptyRowAttr,parent);
    }

    return true;
}

TagXmlItem *TreeXmlModel::rootItem()
{
    return m_rootItem;
}

Qt::ItemFlags TreeXmlModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
                Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsDropEnabled;
}

QVariant TreeXmlModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (m_header[section].isNull())
            return QString("%1").arg(section);
        else
            return  m_header[section];
    }
    return  QVariant();
}

bool TreeXmlModel::setHeaderData(int section, Qt::Orientation orientation,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

QModelIndex TreeXmlModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TagXmlItem *parentItem = toItem(parent);
    TagXmlItem *childItem = parentItem->child(row,m_filterTags,m_attrTags);

    if (childItem){
        return createIndex(row, column, childItem);
    } else
        return QModelIndex();
}

QModelIndex TreeXmlModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TagXmlItem *childItem = toItem(child);
    TagXmlItem *parentItem = childItem->parent();

    return fromItem(parentItem);
}

int TreeXmlModel::rowCount(const QModelIndex &parent) const
{
    TagXmlItem *parentItem = toItem(parent);

    return parentItem->count(m_filterTags,m_attrTags);
}

int TreeXmlModel::rowCount(const QModelIndex &parent, const QStringList &tags) const
{
    TagXmlItem *parentItem = toItem(parent);

    return parentItem->count(tags,m_attrTags);
}

bool TreeXmlModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    QModelIndex index = insertLastRows(row, count, parent);
    if (index.isValid()) {
        m_lastInsRow = index;
        return true;
    }

    return false;

//    TagXmlItem *parentItem = toItem(parent);
//    if (!isInsert(parent))
//        return false;
//    bool success = true;

//    int position = parentItem->count(m_filterTags);

//    for (int i=0;i<count;i++)
//        success = parentItem->insertChild(m_insTag) && success;

//    updateInsertRows(position,count,parent);

//    // Добавление корнего узла
//    if (parent.isValid())
//        m_lastInsRow = parent.child(position+count-1,0);
//    else
//        m_lastInsRow = index(position+count-1,0,parent);

//    return success;
}

QModelIndex TreeXmlModel::insertLastRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    TagXmlItem *parentItem = toItem(parent);
    if (!isInsert(parent))
        return QModelIndex().child(-1,-1);
    bool success = true;

    int position = parentItem->count(m_filterTags);

    for (int i=0;i<count;i++)
        success = parentItem->insertChild(m_insTag) && success;

    updateInsertRows(position,count,parent);

    // Добавление корневого узла
    if (success) {
        if (parent.isValid())
            return parent.child(position+count-1,0);
        else
            return index(position+count-1,0,parent);
    }

    return QModelIndex().child(-1,-1);
}

void TreeXmlModel::updateInsertRows(int row,int count, const QModelIndex &parent)
{
    QModelIndex curIndex = parent.child(row,0);
    if (isAttr(curIndex))
        for (int i=0;i<this->rowCount(parent);i++){
            QModelIndex index = parent.child(i,0);
            if (!isAttr(index))
                updateInsertRows(this->rowCount(index),count,index);
        }
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
}

void TreeXmlModel::updateRemoveRows(int emptyRowAttr,int count, const QModelIndex &parent)
{
    // Если атрибут то обновляем по дереву наследования
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttr(index)){
            updateRemoveRows(emptyRowAttr,count,index);
            int row = this->rowCount(index)-emptyRowAttr+count-1;
            beginRemoveRows(index,row,row+count-1);
            endRemoveRows();
        }
    }
}

bool TreeXmlModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXmlItem *parentItem = toItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    beginRemoveRows(parent,row,row+count-1);
    for (int i=count-1;i>=0;i--)
        parentItem->removeChild(row+i);
    endRemoveRows();

    int emptyRowAttr = 0;
    for (int i=row+1-count;i<this->rowCount(parent);i++){
        QModelIndex index = parent.child(i,0);

        if (isAttr(index))
            emptyRowAttr++;
    }
    updateRemoveRows(emptyRowAttr,count,parent);

    return true;
}

QModelIndex TreeXmlModel::lastInsertRow()
{
    return m_lastInsRow;
}

bool TreeXmlModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(row)

    if (!parent.isValid())
        return false;

    if (action == Qt::IgnoreAction)
        return true;
    if (!data->hasFormat("application/classxmlmodel"))
        return false;
    if (column >= columnCount(parent))
        return false;
    if (isAttr(parent))
        return false;

    const  MimeDataIndex *mimeData
            = qobject_cast<const MimeDataIndex *>(data);
    foreach (const QModelIndex& index, mimeData->indexes()){
        if (index.isValid()) {
            if (action == Qt::MoveAction)
                return moveIndex(index, parent, true, true);
            else
                return copyIndex(index, parent, true);
        }
    }

    return true;
}


void TreeXmlModel::setInsTagName(const QString &tag)
{
    m_insTag = tag;
}

QString TreeXmlModel::insTagName()
{
    return m_insTag;
}

TagXmlItem *TreeXmlModel::toItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagXmlItem *item = static_cast<TagXmlItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QModelIndex TreeXmlModel::fromItem(TagXmlItem *item) const
{
    if (!item || item == m_rootItem)
        return QModelIndex();

    int row = item->parent()->childNumber(item,m_filterTags,m_attrTags);

    return createIndex(row , 0, item);
}

Qt::DropActions TreeXmlModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TreeXmlModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeXmlModel::mimeTypes() const
{
    QStringList types;
    types << "application/classxmlmodel";
    return types;
}

QMimeData *TreeXmlModel::mimeData(const QModelIndexList &indexes) const
{
    PersistentIndexes persistentIndex;

    foreach (const QModelIndex& index,indexes){
        if (index.isValid())
            if (!isInherited(index))
                persistentIndex.append(QPersistentModelIndex(index));
    }

    MimeDataIndex *mimeData = new MimeDataIndex();
    mimeData->setIndexes(persistentIndex);
    return mimeData;
}
