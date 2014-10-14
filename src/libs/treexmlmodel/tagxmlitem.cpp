#include <QtXml>
#include "tagxmlitem.h"

TagXmlItem::TagXmlItem(QDomNode &node,TagXmlItem *parent)
{
    domNode = node;
    parentItem = parent;
}

TagXmlItem::~TagXmlItem()
{
    qDeleteAll(childItems);
    qDeleteAll(contentItems);
}

QDomNode TagXmlItem::node() const
{
    return domNode;
}

QString TagXmlItem::nodeName() const
{
    return domNode.nodeName();
}

bool TagXmlItem::isInherited()
{
    QDomNode parentNode1 = node().parentNode();
    QDomNode parentNode2 = parent()->node();

    if (parentNode1!=parentNode2)
        return true;

    return false;
}

int TagXmlItem::count(QStringList tags,QStringList parenttags){
    int count = 0;
    // Поиск количества потомков
    if (tags.count()>0)
        for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        {
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName))
                    count++;
        }
    else
        count = domNode.childNodes().count();

    // Поиск количества наследуемых потомков
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображать
        QString nodeName = domNode.nodeName();
        if (parenttags.contains(nodeName))
                return count;

        // Считаем количество унаследованных узлов
        if (parentItem != NULL)
            foreach (const QString &tag,parenttags)
                if (tags.contains(tag)){
                    count += parent()->count(parenttags,parenttags);
                    break;
                }
    }

    return count;
}

int TagXmlItem::hasChildren(QStringList tags, QStringList parenttags)
{
    // Поиск количества потомков
    if (tags.count()>0)
        for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        {
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName))
                    return true;
        }
    else if (!domNode.childNodes().isEmpty())
        return true;

    // Поиск количества наследуемых потомков
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображать
        QString nodeName = domNode.nodeName();
        if (parenttags.contains(nodeName))
                return false;

        // Считаем количество унаследованных узлов
        if (parentItem != NULL)
            foreach (const QString &tag,parenttags)
                if (tags.contains(tag)){
                    return parent()->hasChildren(parenttags,parenttags);
                }
    }

    return false;
}

TagXmlItem *TagXmlItem::parent()
{
    return parentItem;
}

TagXmlItem *TagXmlItem::child(int i,QStringList tags, QStringList parenttags){

    int number=0;
    foreach (TagXmlItem *childItem, childItems){
        if (tags.count()>0){
            QString nodeName =  childItem->domNode.nodeName();
            if (tags.contains(nodeName)) {
                if (i==number)
                    return childItem;
                number++;
            }
        } else {
            if (i==number)
                return childItem;
            number++;

        }
    }

    // Поиск узла потомка
    number=0;
    for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        if (tags.count()>0){
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName)){
                if (i==number){
                    TagXmlItem *childItem = new TagXmlItem(childNode, this);
                    childItems.insert(i,childItem);
                    return childItem;
                }
                number++;
            }
        } else {
            if (i==number){
                TagXmlItem *childItem = new TagXmlItem(childNode, this);
                childItems.insert(i,childItem);
                return childItem;
            }
            number++;
        }

    // Поиск наследуемого узла потомка
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображаем
        QString nodeName = domNode.nodeName();
        if (parenttags.contains(nodeName))
                return 0;

        // Поиск наследуемого узла
        if ((i-number)>=0 && parentItem!=NULL){
            TagXmlItem *child = parentItem->child(i-number,parenttags,parenttags);
            TagXmlItem *childItem = new TagXmlItem(child->domNode, this);
            childItem->locationItem = child;
            child->contentItems.append(childItem);

            return childItem;
        }
    }

    return 0;
}

int TagXmlItem::childNumber(TagXmlItem *child,QStringList tags, QStringList parenttags)
{
    int count = 0;
    // Поиск количества потомков

    for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
    {
        if (tags.count()>0){
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName)){
                if (child->domNode == childNode)
                    return count;
                count++;
            }
        }else{
            if (child->domNode == childNode)
                return count;
            count++;
        }
    }

    // Поиск количества наследуемых потомков
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображать
        QString nodeName = domNode.nodeName();
        if (parenttags.contains(nodeName))
            return -1;

        // Считаем количество унаследованных узлов
        if (parentItem != NULL){
            int number = parent()->childNumber(child,parenttags,parenttags);
            if (number!=-1)
                return count + number;
        }
    }

    return -1;
}

bool TagXmlItem::insertChild(const QString &tagname)
{
    QDomElement node = domNode.ownerDocument().createElement(tagname);

    if  (node.isNull())
        return false;

    domNode.appendChild(node);
    TagXmlItem *childItem = new TagXmlItem(node, this);
    childItems.append(childItem);
    return true;
}

QVariant TagXmlItem::value(const QString& attr)
{
    QDomNamedNodeMap attributeMap = domNode.attributes();
    QString value = attributeMap.namedItem(attr).nodeValue();

    if (value == QString("true"))
        return QVariant(true);

    if (value == QString("false"))
        return QVariant(false);

    return value;
}

void TagXmlItem::setValue(const QString& attr, const QVariant &val)
{
    domNode.toElement().setAttribute(attr,val.toString());
}

bool TagXmlItem::insertChild(const QString& tagname, int i, QStringList tags, QStringList parenttags)
{
    TagXmlItem *afterItem = child(i, tags, parenttags);
    if (!afterItem) return false;
    TagXmlItem *parentItem = afterItem->parent();
    if (!parentItem) return false;
    QDomElement node = domNode.ownerDocument().createElement(tagname);

    if  (node.isNull())
        return false;

    domNode.insertBefore(node, afterItem->node());
    TagXmlItem *childItem = new TagXmlItem(node, this);
    childItems.insert(parentItem->childItems.indexOf(afterItem), childItem);
    return true;
}

bool TagXmlItem::removeChild(const int &row)
{ 
    TagXmlItem *childItem = child(row);
    domNode.removeChild(childItem->node());
    delete childItems.takeAt(row);

    return true;
}

bool TagXmlItem::checkRemoveChild(const int &row){

    if (childItems.count() > row)
            return true;

    return false;
}
