#include <QtGui>
#include <QtXml>
#include <QMessageBox>
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

void TreeXMLModel::addTagFilter(QString tag)
{
    m_filterTags << tag;
}

void TreeXMLModel::clearTagsFilter()
{
    m_filterTags.clear();
}

void TreeXMLModel::addAttributeTag(QString tag)
{
    m_attrTags << tag;
}

void TreeXMLModel::clearAttributeTags(){
    m_attrTags.clear();
}

bool TreeXMLModel::isAttribute(const QModelIndex &index) const
{
    TagXMLItem *item = getItem(index);
    QDomNode node = item->node();
    foreach (QString tagName,m_attrTags)
        if (tagName == node.nodeName())
            return true;
    return false;
}

bool TreeXMLModel::isInsert(const QModelIndex &index) const
{
    TagXMLItem *item = getItem(index);
    QDomNode node = item->node();

    if (index.isValid()){
        foreach (QString tagName,m_insertTags[node.nodeName()])
            if (tagName == m_insTag)
                return true;
        return false;
    } else {
        if (rowCount(index) == 0)
            return !isAttribute(index);
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

void TreeXMLModel::addDisplayedAttr(QString nameAttr, QStringList value, QIcon icon)
{
    if (value.count()>m_column)
        m_column = value.count();
    m_displayedAttr[nameAttr] = value;

    if (!icon.isNull())
        m_displayedIcon[nameAttr] = icon;
}

void TreeXMLModel::addInsertTags(QString tag,QStringList value)
{
    m_insertTags.insert(tag,value);
}

int TreeXMLModel::indexDisplayedAttr(QString nameAttr, QString fieldName)
{
    for (int i=0;i<m_displayedAttr[nameAttr].count();i++){
        if (m_displayedAttr[nameAttr].at(i)==fieldName)
            return i;
    }
    return -1;
}


void TreeXMLModel::removeDisplayedAttr(QString nameAttr)
{
    bool flagcolumn = false;
    if (m_displayedAttr[nameAttr].count() == m_column)
        flagcolumn = true;

    m_displayedAttr.remove(nameAttr);
    m_displayedIcon.remove(nameAttr);

    if (flagcolumn){
        m_column = 1;
        foreach (QStringList diplayedAttr, m_displayedAttr.values())
            if (diplayedAttr.count()>m_column)
                m_column = diplayedAttr.count();
    }
}

QVariant TreeXMLModel::data(const QModelIndex &index, int role) const
{

    TagXMLItem *item = getItem(index);
    QDomNode node = item->node();

    if (role == Qt::DecorationRole)
        if (!m_displayedIcon[node.nodeName()].isNull()&&index.column()==0)
            return m_displayedIcon[node.nodeName()];

    if (role == Qt::UserRole){
        return node.nodeName();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){


        if (m_displayedAttr[node.nodeName()].isEmpty())
            return node.nodeName();

        if (index.column()<m_displayedAttr[node.nodeName()].count()){

            QString attrName = m_displayedAttr[node.nodeName()].at(index.column());
            if (attrName == "parent"){
                node = node.parentNode();

                if (!node.isElement())
                    return QVariant();

                attrName = m_displayedAttr[node.nodeName()].at(0);
            }
            QDomNamedNodeMap attributeMap = node.attributes();
            return attributeMap.namedItem(attrName).nodeValue();
        }
    }
    return QVariant();
}

void TreeXMLModel::updateModifyRow(int emptyRowAttr, const QModelIndex &parent){
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttribute(index)){
            updateModifyRow(emptyRowAttr,index);
            int row = this->rowCount(index)-emptyRowAttr-1;
            QModelIndex updateIndex = index.child(row,0);
            emit dataChanged(updateIndex,updateIndex.sibling(updateIndex.row(),columnCount(updateIndex)));
        }
    }
}

bool TreeXMLModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (role != Qt::EditRole) return false;

    TagXMLItem *item = getItem(index);
    QDomNode node = item->node();

    node.toElement().setAttribute(m_displayedAttr[node.nodeName()].at(index.column()),value.toString());
    emit dataChanged(index,index);

    if (isAttribute(index)){
        int emptyRowAttr = 0;
        QModelIndex parent = index.parent();
        for (int i=index.row()+1;i<this->rowCount(parent);i++){
            QModelIndex idx = parent.child(i,parent.column());
            if (isAttribute(idx))
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

    TagXMLItem *parentItem = getItem(parent);

    TagXMLItem *childItem = parentItem->child(row,m_filterTags,m_attrTags);

    if (childItem){
        return createIndex(row, column, childItem);
    }else
        return QModelIndex();
}

QModelIndex TreeXMLModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    TagXMLItem *childItem = getItem(child);
    TagXMLItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();

    int row = parentItem->parent()->childNumber(parentItem,m_filterTags,m_attrTags);

    return createIndex(row , 0, parentItem);
}

int TreeXMLModel::rowCount(const QModelIndex &parent) const
{
    TagXMLItem *parentItem = getItem(parent);

    return parentItem->count(m_filterTags,m_attrTags);
}

bool TreeXMLModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);

    TagXMLItem *parentItem = getItem(parent);
    if (!isInsert(parent))
        return false;
    bool success = true;

    int position = this->rowCount(parent);

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

void TreeXMLModel::updateInsertRows(int row,int count, const QModelIndex &parent){
    QModelIndex curIndex = parent.child(row,0);
    if (isAttribute(curIndex))
        for (int i=0;i<this->rowCount(parent);i++){
            QModelIndex index = parent.child(i,0);
            if (!isAttribute(index))
                updateInsertRows(this->rowCount(index)-count,count,index);
        }
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
}

void TreeXMLModel::updateRemoveRows(int emptyRowAttr,int count, const QModelIndex &parent){
    // Если атрибут то обновляем по дереву наследования
    int rowCount = this->rowCount(parent);
    for (int i=0;i<rowCount;i++){
        QModelIndex index = parent.child(i,0);
        if (!isAttribute(index)){
            updateRemoveRows(emptyRowAttr,count,index);
            int row = this->rowCount(index)-emptyRowAttr+count-1;
            beginRemoveRows(index,row,row+count-1);
            endRemoveRows();
        }
    }
}

bool TreeXMLModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXMLItem *parentItem = getItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    beginRemoveRows(parent,row,row+count-1);
    for (int i=count-1;i>=0;i--)
        parentItem->removeChild(row+i);
    endRemoveRows();

    int emptyRowAttr = 0;
    for (int i=row+1-count;i<this->rowCount(parent);i++){
        QModelIndex index = parent.child(i,0);
        if (isAttribute(index))
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

    // get the data
    QByteArray encodedData = data->data("application/classxmlmodel");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QString tag;
    stream >> tag;
    setInsTagName(tag);

    if (!insertRow(row,parent))
        return false;

    QModelIndex attrParent = lastInsertRow();
    QDomElement node = getItem(lastInsertRow())->node().toElement();
    while (!stream.atEnd()) {
        QString nameAttr;
        stream >> nameAttr;
        if (nameAttr==QString("|")){
            stream >> tag;
            setInsTagName(tag);
            insertRow(row,attrParent);
            node = getItem(lastInsertRow())->node().toElement();
        } else {
            QString value;
            stream >> value;
            node.setAttribute(nameAttr,value);
        }
    }
    return true;
}


void TreeXMLModel::setInsTagName(QString tagName)
{
    m_insTag = tagName;
}

TagXMLItem *TreeXMLModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagXMLItem *item = static_cast<TagXMLItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
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


QMimeData *TreeXMLModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QDomElement node = getItem(index)->node().toElement();
            stream << node.tagName();
            QDomNamedNodeMap attrs = node.attributes();
            for (int i = 0; i < attrs.size(); ++ i) {
                QDomAttr attr = attrs.item(i).toAttr();
                stream << attr.name();
                stream << attr.value();
            }

            // Упаковка входящих атрибутов класса
            if (!isAttribute(index)){
                for (QDomNode childNode = node.firstChild();!childNode.isNull();
                     childNode = childNode.nextSibling()){

                    QDomElement node = childNode.toElement();
                    foreach (QString tagName,m_attrTags)
                        if (tagName == node.tagName()){

                            // Разделитель
                            stream << QString("|");

                            stream << node.tagName();
                            attrs = node.attributes();
                            for (int i = 0; i < attrs.size(); ++ i) {
                                QDomAttr attr = attrs.item(i).toAttr();
                                stream << attr.name();
                                stream << attr.value();
                            }
                            break;
                        }

                }
            }
        }
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("application/classxmlmodel", encodedData);
    return mimeData;
}
