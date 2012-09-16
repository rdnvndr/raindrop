#include <QtXml>
#include "tagxmlitem.h"

TagXMLItem::TagXMLItem(QDomNode &node,TagXMLItem *parent)
{
    domNode = node;
    parentItem = parent;
}

TagXMLItem::~TagXMLItem()
{
    qDeleteAll(childItems);
    qDeleteAll(contentItems);
}

QDomNode TagXMLItem::node() const
{
    return domNode;
}

int TagXMLItem::count(QStringList tags,QStringList parenttags){
    int count = 0;
    // Поиск количества потомков
    if (tags.count()>0)
        for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        {
            foreach (QString tag,tags)
                if (childNode.nodeName()==tag)
                    count++;

        }
    else
        count = domNode.childNodes().count();

    // Поиск количества наследуемых потомков
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображать
        foreach (QString tag,parenttags)
            if (domNode.nodeName()==tag)
                return count;

        // Считаем количество унаследованных узлов
        if (parentItem != NULL)
            count += parent()->count(parenttags,parenttags);
    }

    return count;
}

TagXMLItem *TagXMLItem::parent()
{
    return parentItem;
}

TagXMLItem *TagXMLItem::child(int i,QStringList tags, QStringList parenttags){

    int number=0;
    foreach (TagXMLItem *childItem,childItems){
        if (tags.count()>0){
            foreach (QString tag,tags)
                if (childItem->domNode.nodeName()==tag){
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
            foreach (QString tag,tags)
                if (childNode.nodeName()==tag){
                    if (i==number){
                        TagXMLItem *childItem = new TagXMLItem(childNode, this);
                        childItems.insert(i,childItem);
                        return childItem;
                    }
                    number++;
                }
        }
        else {
            if (i==number){
                TagXMLItem *childItem = new TagXMLItem(childNode, this);
                childItems.insert(i,childItem);
                return childItem;
            }
            number++;
        }

    // Поиск наследуемого узла потомка
    if (parenttags.count()>0){
        // У наследуемых узлов потомков не отображаем

        foreach (QString tag,parenttags)
            if (domNode.nodeName()==tag)
                return 0;
        // Поиск наследуемого узла
        if ((i-number)>=0 && parentItem!=NULL){
            TagXMLItem *child = parentItem->child(i-number,parenttags,parenttags);
            TagXMLItem *childItem = new TagXMLItem(child->domNode, this);
            childItem->locationItem = child;
            child->contentItems.append(childItem);
            //childItems.insert(i,childItem);

            return childItem;
        }
    }

    return 0;
}

int TagXMLItem::childNumber(TagXMLItem *child,QStringList tags, QStringList parenttags)
{
    int count = 0;
    // Поиск количества потомков

    for (QDomNode childNode = domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
    {
        if (tags.count()>0){
            foreach (QString tag,tags)
                if (childNode.nodeName()==tag){
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
        foreach (QString tag,parenttags)
            if (domNode.nodeName()==tag)
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

QString TagXMLItem::value(const QString& attr)
{
    QDomNamedNodeMap attributeMap = domNode.attributes();
    return attributeMap.namedItem(attr).nodeValue();
}

void TagXMLItem::setValue(const QString& attr, const QString& val)
{
    QDomNamedNodeMap attributeMap = domNode.attributes();
    attributeMap.namedItem(attr).setNodeValue(val);
}

bool TagXMLItem::insertChild(const QString& tagname)
{
    QDomElement node = domNode.ownerDocument().createElement(tagname);

    if  (node.isNull())
        return false;

    domNode.appendChild(node);
    TagXMLItem *childItem = new TagXMLItem(node, this);
    childItems.append(childItem);
    return true;
}

bool TagXMLItem::removeChild(const int &row)
{ 
    TagXMLItem *childItem = child(row);
    domNode.removeChild(childItem->node());
    delete childItems.takeAt(row);

    return true;
}

bool TagXMLItem::checkRemoveChild(const int &row){

    if (childItems.count() > row)
            return true;

    return false;
}
