#include <QtXml>
#include <QMessageBox>
#include <QUuid>
#include "tagxmlitem.h"
#include "treexmlhashmodel.h"

TreeXmlHashModel::TreeXmlHashModel(QDomNode document, QObject *parent)
    : TreeXmlModel(document, parent)
{

}

TreeXmlHashModel::~TreeXmlHashModel()
{   
    m_hashValue.clear();
    m_hashAttr.clear();
}

void TreeXmlHashModel::addHashAttr(const QString &tag, const QString &value, UniqueAttr unique)
{
    m_hashAttr[tag].insert(value,unique);
}

void TreeXmlHashModel::makeHashingOne(TagXmlItem *item, bool remove)
{
    QString tag = item->nodeName();
    foreach (const QString& attr, m_hashAttr[tag].keys()){
        QString value = upperValue(tag, attr, item->value(attr).toString());
        if (remove)
            m_hashValue[tag][attr].remove(value,item);
        else
            m_hashValue[tag][attr].insert(value,item);
    }
}

bool TreeXmlHashModel::makeHashingData(const QModelIndex &index, QString &dataValue)
{
    // Обновление хэша контроля уникальности
    if (!isInherited(index)) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = displayedAttr(tag,index.column());
        if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentRename
                || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentUpperRename
                || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParent
                || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentUpper) {
            int row = 0;
            QModelIndex rootIndex = index.parent();
            QModelIndex childIndex = this->index(row, index.column(), rootIndex);
            while (childIndex.isValid())
            {
                if (tag  == childIndex.data(TreeXmlModel::TagRole).toString()
                        && childIndex != index)
                    if (upperValue(tag, attr, dataValue)
                            == upperValue(tag, attr, childIndex.data().toString())) {
                        if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentRename
                                || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentUpperRename){
                            int position = dataValue.lastIndexOf(QRegExp("_\\d*$"));
                            int number = 1;
                            if (position != -1)
                                number = dataValue.mid(position+1).toInt()+1;
                            dataValue = dataValue.left(position)+QString("_%1").arg(number);
                            row = 0;
                            continue;
                        } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParent
                                   || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueParentUpper) {
                            return false;
                        }
                    }
                childIndex = this->index(++row, index.column(), rootIndex);
            }
        } else if (m_hashAttr[tag].contains(attr)){
            QModelIndex existIndex = indexHashAttr(tag,attr,dataValue);
            while (existIndex.isValid() && existIndex!=index) {
                if (m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueRename
                        || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueUpperRename){
                    int position = dataValue.lastIndexOf(QRegExp("_\\d*$"));
                    int number = 1;
                    if (position != -1)
                        number = dataValue.mid(position+1).toInt()+1;
                    dataValue = dataValue.left(position)+QString("_%1").arg(number);
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Unique
                           || m_hashAttr[tag][attr] == TreeXmlHashModel::UniqueUpper) {
                    return false;
                } else if (m_hashAttr[tag][attr] == TreeXmlHashModel::Uuid) {
                    dataValue = QUuid::createUuid().toString();
                } else
                    break;
                existIndex = indexHashAttr(tag,attr,dataValue);
            }
            TagXmlItem *item = toItem(index);

            m_hashValue[tag][attr].remove(upperValue(tag, attr,
                                                     // Возможно надо заменить на
                                                     //   item->value(attr)
                                                     item->node().toElement().attribute(attr)),item);
            m_hashValue[tag][attr].insert(upperValue(tag, attr, dataValue),item);
        }
    }
    return true;
}

void TreeXmlHashModel::insertUuid(const QModelIndex &index, QString tag)
{
    QString attr = uuidAttr(tag);
    if (!attr.isEmpty()) {
        TagXmlItem *item = toItem(index);
        QString value = QUuid::createUuid().toString();
        item->setValue(attr,value);
        m_hashValue[tag][attr].insert(upperValue(tag, attr, value),item);
    }
}

void TreeXmlHashModel::makeHashing(TagXmlItem *item, bool remove)
{    
    makeHashingOne(item, remove);
    for (int row=0; row < item->count(tagsFilter()); row++) {
        TagXmlItem *childItem = item->child(row,tagsFilter());
        makeHashing(childItem, remove);
    }
}

QModelIndex TreeXmlHashModel::indexHashAttr(const QString &tag, const QString &attr,
                                         const QVariant &value, int number) const
{
    QString dataValue = upperValue(tag, attr, value.toString());

    int column = columnDisplayedAttr(tag,attr);
    if (number <  m_hashValue[tag][attr].values(dataValue).count()) {
        QModelIndex index = fromItem(
                    m_hashValue[tag][attr].values(
                        dataValue).at(number)
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
    QString tag = index.data(TreeXmlModel::TagRole).toString();
    QString attr = displayedAttr(tag,index.column());
    if (m_hashAttr[tag].contains(attr)){
        QString value = upperValue(tag, attr, index.data().toString());
        if (remove)
            m_hashValue[tag][attr].remove(value, toItem(index));
        else
            m_hashValue[tag][attr].insert(value, toItem(index));
    }
}

void TreeXmlHashModel::addRelation(const QString &tag, const QString &attr,
                               const QString &linkTag, const QString &linkAttr)
{
    m_linkAttr[tag][attr][linkTag] = linkAttr;
}

QList<TreeXmlHashModel::TagWithAttr> TreeXmlHashModel::fromRelation(const QString &linkTag,
                                                                      const QString &linkAttr) const
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

TreeXmlHashModel::TagWithAttr TreeXmlHashModel::toRelation(const QString &tag,
                                                             const QString &attr) const
{
    TreeXmlHashModel::TagWithAttr tagWithAttr;
    foreach (const QString &linkTag,m_linkAttr[tag][attr].keys()) {
        tagWithAttr.tag =  linkTag;
        tagWithAttr.attr = m_linkAttr[tag][attr][linkTag];
        return tagWithAttr;
    }
    return tagWithAttr;
}

QModelIndex TreeXmlHashModel::indexLink(const QString &tag, const QString &attr, const QVariant &value) const
{
    if (m_linkAttr[tag].contains(attr)) {
        foreach (const QString &linkTag,m_linkAttr[tag][attr].keys()){
            QString linkAttr = m_linkAttr[tag][attr][linkTag];
            QString attr = uuidAttr(linkTag);
            if (!attr.isEmpty()) {
                QModelIndex linkIndex = indexHashAttr(linkTag, attr, value);
                int column = columnDisplayedAttr(linkTag,linkAttr);
                return linkIndex.sibling(linkIndex.row(),column);
            }
        }
    }
    return QModelIndex();
}

QModelIndex TreeXmlHashModel::indexLink(const QModelIndex &index) const
{
    TagXmlItem *item = toItem(index);
    QString tag = item->nodeName();
    QString attrName = displayedAttr(tag,index.column());
    QVariant value = item->value(attrName);

    return indexLink(tag, attrName, value);
}

QVariant TreeXmlHashModel::data(const QModelIndex &index, int role) const
{
    TagXmlItem *item = toItem(index);
    QString tag = item->nodeName();

    if ((role == Qt::DisplayRole || role == Qt::EditRole)
            && !displayedAttr(tag,0).isEmpty())
    {
        if (!displayedAttr(tag,index.column()).isEmpty()){
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
        return QVariant();
    }

    return TreeXmlModel::data(index, role);
}

bool TreeXmlHashModel::setData(const QModelIndex &index, const QVariant &value,
                           int role)
{
    if (role != Qt::EditRole) return false;

    TagXmlItem *item = toItem(index);
    QString attrName = displayedAttr(item->nodeName(),index.column());
    QString dataValue = value.toString();
    if (attrName == QString("parent"))
        dataValue = data(index,Qt::EditRole).toString();

    if (!makeHashingData(index, dataValue))
        return false;

    return TreeXmlModel::setData(index,dataValue,role);
}

bool TreeXmlHashModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return insertLastRows(row,count,parent).isValid();
}

QModelIndex TreeXmlHashModel::insertLastRows(int row, int count,
                                             const QModelIndex &parent, QString tag)
{
    QModelIndex lastInsertRow = TreeXmlModel::insertLastRows(row, count, parent,tag);
    if (!lastInsertRow.isValid())
        return QModelIndex().child(-1,-1);

    for (int i=0;i<count;i++) {
        insertUuid(index(row+i,0,parent),tag);
    }

    return lastInsertRow;
}

bool TreeXmlHashModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TagXmlItem *parentItem = toItem(parent);

    if  (!parentItem->checkRemoveChild(row))
        return false;

    // Удаление хэша уникальности
    for (int i=row; i < row+count; i++){
        QModelIndex childIndex = index(i,0,parent);
        if (!isInherited(childIndex))
            makeHashing(toItem(childIndex),true);
    }

    return TreeXmlModel::removeRows(row, count, parent);
}

bool TreeXmlHashModel::moveIndex(const QModelIndex &srcIndex, const QModelIndex &destIndex,
                                 int row, bool recursively)
{
    if (isInherited(srcIndex))
        return false;

    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();

    QModelIndex index = insertLastRows((row >= 0) ? row : 0, 1, destIndex, tag);
    if (!index.isValid())
        return false;

    for (int i = 0; i<this->columnCount(srcIndex); i++) {
        QVariant value = srcIndex.sibling(srcIndex.row(), i).data(Qt::EditRole);
        QString nameAttr = displayedAttr(tag, i);
        QModelIndex existIndex = indexHashAttr(tag,nameAttr,value);
        if (existIndex.isValid())
            refreshHashingOne(existIndex,true);

        TreeXmlModel::setData(index.sibling(index.row(), i), value);
    }

    bool success = true;
    if (recursively) {
        int row = 0;
        QModelIndex childIndex = srcIndex.child(row,0);
        while (childIndex.isValid())
        {
            if (!isInherited(childIndex))
                success = moveIndex(childIndex, index, row, recursively) && success;
            childIndex = srcIndex.child(++row,0);
        }
    }

    return success;
}

QString TreeXmlHashModel::uuidAttr(const QString &tag) const
{
    foreach (QString attr,m_hashAttr[tag].keys())
        if (m_hashAttr[tag][attr]==TreeXmlHashModel::Uuid)
            return attr;
    return QString();
}

QString TreeXmlHashModel::upperValue(const QString &tag, const QString &attr,
                                     const QString &value) const
{
    TreeXmlHashModel::UniqueAttr typeAttr = m_hashAttr[tag][attr];
    if (typeAttr == TreeXmlHashModel::UniqueUpperRename
            || typeAttr == TreeXmlHashModel::UniqueUpper
            || typeAttr == TreeXmlHashModel::UniqueParentUpperRename
            || typeAttr == TreeXmlHashModel::UniqueParentUpper) {
        return value.toUpper();
    }
    return value;
}
