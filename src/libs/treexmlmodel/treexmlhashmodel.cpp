#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "tagxmlitem.h"
#include "treexmlhashmodel.h"

TreeXmlHashModel::TreeXmlHashModel(QDomNode document, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TagXMLItem(document, NULL);
    m_column = 1;
    m_insTag = "element";
}

TreeXmlHashModel::~TreeXmlHashModel()
{   
    m_hashValue.clear();
    m_hashAttr.clear();
    delete m_rootItem;
}

int TreeXmlHashModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return  m_column;
}

void TreeXmlHashModel::addTagFilter(const QString &tag)
{
    m_filterTags << tag;
}

void TreeXmlHashModel::clearTagsFilter()
{
    m_filterTags.clear();
}

void TreeXmlHashModel::addAttrTag(const QString &tag)
{
    m_attrTags << tag;
}

void TreeXmlHashModel::clearAttrTags(){
    m_attrTags.clear();
}

bool TreeXmlHashModel::isInherited(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    return item->isInherited();
}

void TreeXmlHashModel::addHashAttr(const QString &tag, const QString &value, UniqueAttr unique)
{
    m_hashAttr[tag].insert(value,unique);
}

void TreeXmlHashModel::makeHashingOne(TagXMLItem *item, bool remove)
{
    QString tag = item->nodeName();
    foreach (const QString& attr, m_hashAttr[tag].keys()){
        if (remove)
            m_hashValue[tag][attr].remove(item->value(attr),item);
        else
            m_hashValue[tag][attr].insert(item->value(attr),item);
    }
}

bool TreeXmlHashModel::makeHashingData(const QModelIndex &index, QString &dataValue)
{
    // Обновление хэша контроля уникальности
    if (!isInherited(index)) {
        QString tag  = index.data(Qt::UserRole).toString();
        QString attr = displayedAttr(tag,index.column());
        if (m_hashAttr[tag].contains(attr)){
            QModelIndex existIndex = indexHashAttr(tag,attr,dataValue);
            while (existIndex.isValid() && existIndex!=index) {
                if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueRename){
                    int position = dataValue.lastIndexOf(QRegExp("_\\d*$"));
                    int number = 1;
                    if (position != -1)
                        number = dataValue.mid(position+1).toInt()+1;
                    dataValue = dataValue.left(position)+QString("_%1").arg(number);
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Unique) {
                    return false;
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Uuid) {
                    dataValue = QUuid::createUuid().toString();
                } else
                    break;
                existIndex = indexHashAttr(tag,attr,dataValue);
            }
            TagXMLItem *item = toItem(index);

            m_hashValue[tag][attr].remove(
                        item->node().toElement().attribute(attr),item);
            m_hashValue[tag][attr].insert(dataValue,item);
        }
    }
    return true;
}

void TreeXmlHashModel::insertUuid(const QModelIndex &index)
{
    QString attr = uuidAttr(m_insTag);
    if (!attr.isEmpty()) {
        TagXMLItem *item = toItem(index);
        QString value = QUuid::createUuid().toString();
        item->setValue(attr,value);
        m_hashValue[m_insTag][attr].insert(value,item);
    }
}

void TreeXmlHashModel::makeHashing(TagXMLItem *item, bool remove)
{    
    makeHashingOne(item, remove);
    for (int row=0; row < item->count(m_filterTags); row++) {
        TagXMLItem *childItem = item->child(row,m_filterTags);
        makeHashing(childItem, remove);
    }
}

QModelIndex TreeXmlHashModel::indexHashAttr(const QString &tag, const QString &attr,
                                         const QVariant &value, int number) const
{
    int column = columnDisplayedAttr(tag,attr);
    if (number <  m_hashValue[tag][attr].values(value.toString()).count()) {
        QModelIndex index = fromItem(
                    m_hashValue[tag][attr].values(
                        value.toString()).at(number)
                    );
        return index.sibling(index.row(),column);
    }
    return QModelIndex();
}

void TreeXmlHashModel::refreshHashing(const QModelIndex &index, bool remove)
{
    makeHashing(toItem(index),remove);
}

void TreeXmlHashModel::refreshHashingOne(const QModelIndex &index, bool remove)
{
    QString tag = index.data(Qt::UserRole).toString();
    QString attr = displayedAttr(tag,index.column());
    if (m_hashAttr[tag].contains(attr)){
        if (remove )
            m_hashValue[tag][attr].remove(index.data().toString(),toItem(index));
        else
            m_hashValue[tag][attr].insert(index.data().toString(),toItem(index));
    }
}

void TreeXmlHashModel::addRelation(const QString &tag, const QString &attr,
                               const QString &linkTag, const QString &linkAttr)
{
    m_linkAttr[tag][attr][linkTag] = linkAttr;
}

QList<TreeXmlHashModel::TagWithAttr> TreeXmlHashModel::fromRelation(const QString &linkTag, const QString &linkAttr)
{
    QList<TreeXmlHashModel::TagWithAttr> list;
    foreach (const QString &tag,m_linkAttr.keys())
        foreach (const QString &attr,m_linkAttr[tag].keys())
            foreach (const QString &lTag,m_linkAttr[tag][attr].keys())
                if (lTag == linkTag
                        && (m_linkAttr[tag][attr][lTag] == linkAttr || linkAttr.isEmpty()))
                {
                    TreeXmlHashModel::TagWithAttr tagWithAttr;
                    tagWithAttr.tag =  tag;
                    tagWithAttr.attr = attr;
                    list << tagWithAttr;
                }
    return list;
}

TreeXmlHashModel::TagWithAttr TreeXmlHashModel::toRelation(const QString &tag, const QString &attr)
{
    TreeXmlHashModel::TagWithAttr tagWithAttr;
    foreach (const QString &linkTag,m_linkAttr[tag][attr].keys()) {
        tagWithAttr.tag =  linkTag;
        tagWithAttr.attr = m_linkAttr[tag][attr][linkTag];
        return tagWithAttr;
    }
    return tagWithAttr;
}

QModelIndex TreeXmlHashModel::indexLink(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    QString tag = item->nodeName();
    QString attrName = displayedAttr(tag,index.column());

    if (m_linkAttr[tag].contains(attrName)) {
        foreach (const QString &linkTag,m_linkAttr[tag][attrName].keys()){
            QString linkAttr = m_linkAttr[tag][attrName][linkTag];
            QString attr = uuidAttr(linkTag);
            if (!attr.isEmpty()) {
                QModelIndex linkIndex = indexHashAttr(linkTag, attr, item->value(attrName));
                int column = columnDisplayedAttr(linkTag,linkAttr);
                return linkIndex.sibling(linkIndex.row(),column);
            }
        }
    }
    return QModelIndex();
}

bool TreeXmlHashModel::isAttr(const QModelIndex &index) const
{
    TagXMLItem *item = toItem(index);
    foreach (const QString &tagName,m_attrTags)
        if (tagName == item->nodeName())
            return true;
    return false;
}

bool TreeXmlHashModel::isInsert(const QModelIndex &index) const
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

bool TreeXmlHashModel::hasChildren(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return true;
}

bool TreeXmlHashModel::unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move)
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

            QModelIndex existIndex = indexHashAttr(tag,nameAttr,value);
            if (existIndex.isValid() && move)
                refreshHashingOne(existIndex,true);

            if (!setData(index.sibling(index.row(),column),value)){
                nextTag = true;
                removeRow(index.row(),index.parent());
            }
        }
    }
    return true;
}

void TreeXmlHashModel::packData(const QModelIndex &parent, QDataStream &stream) const
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

void TreeXmlHashModel::addDisplayedAttr(const QString &tag, const QStringList &value, QIcon icon)
{
    if (value.count()>m_column)
        m_column = value.count();
    m_displayedAttr[tag] = value;

    if (!icon.isNull())
        m_displayedIcon[tag] = icon;
}

void TreeXmlHashModel::addInsertTags(const QString &tag, const QStringList &value)
{
    m_insertTags.insert(tag,value);
}

int TreeXmlHashModel::columnDisplayedAttr(const QString &tag, const QString &attr) const
{
    for (int i=0;i<m_displayedAttr[tag].count();i++){
        if (m_displayedAttr[tag].at(i)==attr)
            return i;
    }
    return -1;
}

QString TreeXmlHashModel::displayedAttr(const QString &tag, int column) const
{
    if (m_displayedAttr[tag].count()<=column)
        return QString("");
    return m_displayedAttr[tag].at(column);
}

void TreeXmlHashModel::removeDisplayedAttr(const QString &tag)
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

QVariant TreeXmlHashModel::data(const QModelIndex &index, int role) const
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
            if (attrName == "parent"){
                QDomNode nodeParent = item->node().parentNode();
                QString parentTag = nodeParent.nodeName();
                if (!nodeParent.isElement())
                    return QVariant();

                // Отображает в качестве родителя первое поле
                QString attr = uuidAttr(parentTag);
                if (!attr.isEmpty()) {
                    if (role == Qt::DisplayRole)
                        attr = m_linkAttr[tag][attrName][parentTag];
                    return nodeParent.toElement().attribute(attr);
                }
            }

            if (role == Qt::DisplayRole){
                QModelIndex link = indexLink(index);
                if (link.isValid())
                    return link.data();
            }

            return item->value(attrName);
        }
    }
    return QVariant();
}

void TreeXmlHashModel::updateModifyRow(int emptyRowAttr, const QModelIndex &parent)
{
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttr(index)){
            updateModifyRow(emptyRowAttr,index);
            int row = this->rowCount(index)-emptyRowAttr-1;
            QModelIndex updateIndex = index.child(row,0);
            emit dataChanged(updateIndex,updateIndex.sibling(updateIndex.row(),columnCount(updateIndex)));
        }
    }
}

bool TreeXmlHashModel::setData(const QModelIndex &index, const QVariant &value,
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

    if (!makeHashingData(index,dataValue))
        return false;

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

TagXMLItem *TreeXmlHashModel::rootItem()
{
    return m_rootItem;
}

Qt::ItemFlags TreeXmlHashModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
                Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsDropEnabled;
}

QVariant TreeXmlHashModel::headerData(int section, Qt::Orientation orientation,
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

bool TreeXmlHashModel::setHeaderData(int section, Qt::Orientation orientation,
                                 const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

QModelIndex TreeXmlHashModel::index(int row, int column, const QModelIndex &parent)
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

QModelIndex TreeXmlHashModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TagXMLItem *childItem = toItem(child);
    TagXMLItem *parentItem = childItem->parent();

    return fromItem(parentItem);
}

int TreeXmlHashModel::rowCount(const QModelIndex &parent) const
{
    TagXMLItem *parentItem = toItem(parent);

    return parentItem->count(m_filterTags,m_attrTags);
}

int TreeXmlHashModel::rowCount(const QModelIndex &parent, const QStringList &tags) const
{
    TagXMLItem *parentItem = toItem(parent);

    return parentItem->count(tags,m_attrTags);
}

bool TreeXmlHashModel::insertRows(int row, int count, const QModelIndex &parent)
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

    for (int i=0;i<count;i++) {
        insertUuid(index(position+i,0,parent));
    }

    return success;
}

void TreeXmlHashModel::updateInsertRows(int row,int count, const QModelIndex &parent)
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

void TreeXmlHashModel::updateRemoveRows(int emptyRowAttr,int count, const QModelIndex &parent)
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

bool TreeXmlHashModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXMLItem *parentItem = toItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    // Удаление хэша уникальности
    for (int i=row; i < row+count; i++){
        QModelIndex childIndex = index(i,0,parent);
        if (!isInherited(childIndex))
            makeHashing(toItem(childIndex),true);
    }

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

QModelIndex TreeXmlHashModel::lastInsertRow()
{
    return m_lastInsRow;
}

bool TreeXmlHashModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
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


void TreeXmlHashModel::setInsTagName(const QString &tag)
{
    m_insTag = tag;
}

TagXMLItem *TreeXmlHashModel::toItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagXMLItem *item = static_cast<TagXMLItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

QModelIndex TreeXmlHashModel::fromItem(TagXMLItem *item) const
{
    if (!item || item == m_rootItem)
        return QModelIndex();

    int row = item->parent()->childNumber(item,m_filterTags,m_attrTags);

    return createIndex(row , 0, item);
}

Qt::DropActions TreeXmlHashModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions TreeXmlHashModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeXmlHashModel::mimeTypes() const
{
    QStringList types;
    types << "application/classxmlmodel";
    return types;
}

QMimeData *TreeXmlHashModel::mimeData(const QModelIndexList &indexes)
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

QString TreeXmlHashModel::uuidAttr(const QString &tag) const
{
    foreach (QString attr,m_hashAttr[tag].keys())
        if (m_hashAttr[tag][attr]==TreeXmlHashModel::Uuid)
            return attr;
    return QString();
}
