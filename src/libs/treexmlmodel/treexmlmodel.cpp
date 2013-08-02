#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "tagxmlitem.h"
#include "treexmlmodel.h"

TreeXMLModel::TreeXMLModel(QDomNode document, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TagXMLItem(document, NULL);
    m_column = 1;
    m_insTag = "element";
}

TreeXMLModel::~TreeXMLModel()
{
    delete m_rootItem;
}

int TreeXMLModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return  m_column;
}

void TreeXMLModel::addTagFilter(const QString &tag)
{
    m_filterTags << tag;
}

QStringList TreeXMLModel::tagsFilter()
{
    return m_filterTags;
}

void TreeXMLModel::clearTagsFilter()
{
    m_filterTags.clear();
}

void TreeXMLModel::addAttrTag(const QString &tag)
{
    m_attrTags << tag;
}

void TreeXMLModel::clearAttrTags(){
    m_attrTags.clear();
}

bool TreeXMLModel::isInherited(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    return item->isInherited();
}

bool TreeXMLModel::isAttr(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    foreach (const QString &tagName,m_attrTags)
        if (tagName == item->nodeName())
            return true;
    return false;
}

bool TreeXMLModel::isInsert(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);

    if (index.isValid()){
        foreach (const QString &tagName,m_insertTags[item->nodeName()])
            if (tagName == m_insTag)
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

bool TreeXMLModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return true;
}

bool TreeXMLModel::unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move)
{
    QString tag;
    QModelIndex index;
    bool nextTag = false;

    while (!stream.atEnd()) {
        QString nameAttr;
        stream >> nameAttr;
        if (nameAttr==QString("^")){
            stream >> tag;
            setInsTagName(tag);
            if (insertRow(row,parent))
                nextTag = false;
            else
                nextTag = true;
            index = lastInsertRow();
        } else if (nameAttr==QString("{")) {
            unpackData(lastInsertRow(),stream,row);
        } else if (nameAttr==QString("}")) {
            return true;
        } else if (!nextTag){
            QVariant value;
            stream >> value;
            int column = columnDisplayedAttr(tag,nameAttr);
/*hash
            QModelIndex existIndex = indexHashAttr(tag,nameAttr,value);
            if (existIndex.isValid() && move)
                refreshHashingOne(existIndex,true);
*/
            if (!setData(index.sibling(index.row(),column),value)){
                nextTag = true;
                removeRow(index.row(),index.parent());
            }
        }
    }
    return true;
}

void TreeXMLModel::packData(const QModelIndex &parent, QDataStream &stream) const
{
    bool isFirstNode=true;

    for (int row=0;row<rowCount(parent);row++)
    {
        QModelIndex childIndex = parent.child(row,0);
        // Разделитель
        if (isFirstNode){
            stream << QString("{");
            isFirstNode = false;
        }

        if (childIndex.isValid())
            if (!isInherited(childIndex)) {
                stream << QString("^");
                QString tag = data(childIndex,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < columnCount(childIndex); i++) {
                    QString attrName = displayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << childIndex.sibling(childIndex.row(),i).data(Qt::EditRole);
                    }
                }
                if (!isAttr(childIndex))
                    packData(childIndex,stream);
            }
    }

    if (!isFirstNode)
        stream << QString("}");
}

void TreeXMLModel::addDisplayedAttr(const QString &tag, const QStringList &value, QIcon icon)
{
    if (value.count()>m_column)
        m_column = value.count();
    m_displayedAttr[tag] = value;

    if (!icon.isNull())
        m_displayedIcon[tag] = icon;
}

void TreeXMLModel::addInsertTags(const QString &tag, const QStringList &value)
{
    m_insertTags.insert(tag,value);
}

int TreeXMLModel::columnDisplayedAttr(const QString &tag, const QString &attr) const
{
    for (int i=0;i<m_displayedAttr[tag].count();i++){
        if (m_displayedAttr[tag].at(i)==attr)
            return i;
    }
    return -1;
}

QString TreeXMLModel::displayedAttr(const QString &tag, int column) const
{
    if (m_displayedAttr[tag].count()<=column)
        return QString("");
    return m_displayedAttr[tag].at(column);
}

void TreeXMLModel::removeDisplayedAttr(const QString &tag)
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

QVariant TreeXMLModel::data(const QModelIndex &index, int role) const
{
    TagXMLItem *item = toItem(index);
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

void TreeXMLModel::updateModifyRow(int emptyRowAttr, const QModelIndex &parent)
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

bool TreeXMLModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (role != Qt::EditRole) return false;

    TagXMLItem *item = toItem(index);

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

TagXMLItem *TreeXMLModel::rootItem()
{
    return m_rootItem;
}

Qt::ItemFlags TreeXMLModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
                Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsDropEnabled;
}

QVariant TreeXMLModel::headerData(int section, Qt::Orientation orientation,
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

bool TreeXMLModel::setHeaderData(int section, Qt::Orientation orientation,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

QModelIndex TreeXMLModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TagXMLItem *parentItem = toItem(parent);
    TagXMLItem *childItem = parentItem->child(row,m_filterTags,m_attrTags);

    if (childItem){
        return createIndex(row, column, childItem);
    } else
        return QModelIndex();
}

QModelIndex TreeXMLModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TagXMLItem *childItem = toItem(child);
    TagXMLItem *parentItem = childItem->parent();

    return fromItem(parentItem);
}

int TreeXMLModel::rowCount(const QModelIndex &parent) const
{
    TagXMLItem *parentItem = toItem(parent);

    return parentItem->count(m_filterTags,m_attrTags);
}

int TreeXMLModel::rowCount(const QModelIndex &parent, const QStringList &tags) const
{
    TagXMLItem *parentItem = toItem(parent);

    return parentItem->count(tags,m_attrTags);
}

bool TreeXMLModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    TagXMLItem *parentItem = toItem(parent);
    if (!isInsert(parent))
        return false;
    bool success = true;

    int position = parentItem->count(m_filterTags);

    for (int i=0;i<count;i++)
        success = parentItem->insertChild(m_insTag) && success;

    updateInsertRows(position,count,parent);

    // Добавление корнего узла
    if (parent.isValid())
        m_lastInsRow = parent.child(position+count-1,0);
    else
        m_lastInsRow = index(position+count-1,0,parent);

    return success;
}

void TreeXMLModel::updateInsertRows(int row,int count, const QModelIndex &parent)
{
    QModelIndex curIndex = parent.child(row,0);
    if (isAttr(curIndex))
        for (int i=0;i<this->rowCount(parent);i++){
            QModelIndex index = parent.child(i,0);
            if (!isAttr(index))
                updateInsertRows(this->rowCount(index)-count,count,index);
        }
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
}

void TreeXMLModel::updateRemoveRows(int emptyRowAttr,int count, const QModelIndex &parent)
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

bool TreeXMLModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXMLItem *parentItem = toItem(parent);

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

QModelIndex TreeXMLModel::lastInsertRow()
{
    return m_lastInsRow;
}

bool TreeXMLModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                int row, int column, const QModelIndex &parent)
{
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

    QByteArray encodedData = data->data("application/classxmlmodel");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    return unpackData(parent,stream,row,
                      data->parent() == this
                      && action == Qt::MoveAction);
}


void TreeXMLModel::setInsTagName(const QString &tag)
{
    m_insTag = tag;
}

QString TreeXMLModel::insTagName()
{
    return m_insTag;
}

TagXMLItem *TreeXMLModel::toItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagXMLItem *item = static_cast<TagXMLItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QModelIndex TreeXMLModel::fromItem(TagXMLItem *item) const
{
    if (!item || item == m_rootItem)
        return QModelIndex();

    int row = item->parent()->childNumber(item,m_filterTags,m_attrTags);

    return createIndex(row , 0, item);
}

Qt::DropActions TreeXMLModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TreeXMLModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeXMLModel::mimeTypes() const
{
    QStringList types;
    types << "application/classxmlmodel";
    return types;
}

QMimeData *TreeXMLModel::mimeData(const QModelIndexList &indexes)
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex& index,indexes){
        if (index.isValid())
            if (!isInherited(index)){
                stream << QString("^");
                QString tag = data(index,Qt::UserRole).toString();
                stream << tag;
                for (int i = 0; i < columnCount(index); i++) {
                    QString attrName = displayedAttr(tag,i);
                    if (!attrName.isEmpty()){
                        stream << attrName;
                        stream << index.sibling(index.row(),i).data(Qt::EditRole);
                    }
                }
                if (!isAttr(index))
                    packData(index,stream);
            }
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/classxmlmodel", encodedData);
    mimeData->setParent(this);
    return mimeData;
}
