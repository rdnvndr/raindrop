#include <QtXml>
#include "tagxmlitem.h"

TagXmlItem::TagXmlItem(QDomNode &node,TagXmlItem *parent): QObject(parent)
{
    m_domNode = node;
    m_parentItem = parent;
}

TagXmlItem::~TagXmlItem()
{
    emit destroyedItem(this);
    qDeleteAll(m_childItems);
    qDeleteAll(m_inherited);
}

QDomNode TagXmlItem::node() const
{
    return m_domNode;
}

QString TagXmlItem::nodeName() const
{
    return m_domNode.nodeName();
}

void TagXmlItem::setNodeName(const QString &tagName)
{
    m_domNode.toElement().setTagName(tagName);
}

bool TagXmlItem::isInherited()
{
    QDomNode parentNode1 = node().parentNode();
    QDomNode parentNode2 = parent()->node();

    if (parentNode1!=parentNode2)
        return true;

    return false;
}

void TagXmlItem::removeInheritedItem(TagXmlItem *parent)
{
    m_inherited.remove(parent);
}

int TagXmlItem::count(QStringList tags, QStringList parentTags){
    int count = 0;
    // Поиск количества потомков
    if (!tags.empty())
        for (QDomNode childNode = m_domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        {
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName))
                    count++;
        }
    else
        count = m_domNode.childNodes().count();

    // Поиск количества наследуемых потомков
    if (!parentTags.empty()){
        // У наследуемых узлов потомков не отображать
        QString nodeName = m_domNode.nodeName();
        if (parentTags.contains(nodeName))
                return count;

        // Считаем количество унаследованных узлов
        if (m_parentItem != NULL)
            foreach (const QString &tag,parentTags)
                if (tags.contains(tag)){
                    count += parent()->count(parentTags,parentTags);
                    break;
                }
    }

    return count;
}

int TagXmlItem::hasChildren(QStringList tags, QStringList parentTags)
{
    // Поиск количества потомков
    if (!tags.empty())
        for (QDomNode childNode = m_domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        {
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName))
                    return true;
        }
    else if (!m_domNode.childNodes().isEmpty())
        return true;

    // Поиск количества наследуемых потомков
    if (!parentTags.empty()){
        // У наследуемых узлов потомков не отображать
        QString nodeName = m_domNode.nodeName();
        if (parentTags.contains(nodeName))
                return false;

        // Считаем количество унаследованных узлов
        if (m_parentItem != NULL)
            foreach (const QString &tag,parentTags)
                if (tags.contains(tag)){
                    return parent()->hasChildren(parentTags,parentTags);
                }
    }

    return false;
}

TagXmlItem *TagXmlItem::parent()
{
    return m_parentItem;
}

TagXmlItem *TagXmlItem::child(int i, QStringList tags, QStringList parentTags){

    int number=0;
    foreach (TagXmlItem *childItem, m_childItems){
        if (!tags.empty()){
            QString nodeName =  childItem->m_domNode.nodeName();
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
    for (QDomNode childNode = m_domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
        if (!tags.empty()){
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName)){
                if (i==number){
                    TagXmlItem *childItem = new TagXmlItem(childNode, this);
                    m_childItems.insert(i,childItem);
                    return childItem;
                }
                number++;
            }
        } else {
            if (i==number){
                TagXmlItem *childItem = new TagXmlItem(childNode, this);
                m_childItems.insert(i,childItem);
                return childItem;
            }
            number++;
        }

    // Поиск наследуемого узла потомка
    if (!parentTags.empty()){
        // У наследуемых узлов потомков не отображаем
        QString nodeName = m_domNode.nodeName();
        if (parentTags.contains(nodeName))
                return 0;

        // Поиск наследуемого узла
        if ((i-number)>=0 && m_parentItem!=NULL){
            TagXmlItem *child = m_parentItem->child(i-number,parentTags,parentTags);
            if (child) {
                TagXmlItem *childItem;
                if (!child->m_inherited.contains(this)) {
                    childItem = new TagXmlItem(child->m_domNode, this);
                    child->m_inherited.insert(this, childItem);
                    connect(this, SIGNAL(destroyedItem(TagXmlItem*)),
                            child, SLOT(removeInheritedItem(TagXmlItem*)));

                } else
                    childItem = child->m_inherited.value(this);

                return childItem;
            }

        }
    }

    return 0;
}

int TagXmlItem::childNumber(TagXmlItem *child, QStringList tags, QStringList parentTags)
{
    int count = 0;
    // Поиск количества потомков

    for (QDomNode childNode = m_domNode.firstChild();!childNode.isNull();childNode = childNode.nextSibling())
    {
        if (!tags.empty()){
            QString nodeName = childNode.nodeName();
            if (tags.contains(nodeName)){
                if (child->m_domNode == childNode)
                    return count;
                count++;
            }
        }else{
            if (child->m_domNode == childNode)
                return count;
            count++;
        }
    }

    // Поиск количества наследуемых потомков
    if (!parentTags.empty()){
        // У наследуемых узлов потомков не отображать
        QString nodeName = m_domNode.nodeName();
        if (parentTags.contains(nodeName))
            return -1;

        // Считаем количество унаследованных узлов
        if (m_parentItem != NULL){
            int number = parent()->childNumber(child,parentTags,parentTags);
            if (number!=-1)
                return count + number;
        }
    }

    return -1;
}

bool TagXmlItem::insertChild(const QString &tagName)
{
    QDomElement node = m_domNode.ownerDocument().createElement(tagName);

    if  (node.isNull())
        return false;

    m_domNode.appendChild(node);
    TagXmlItem *childItem = new TagXmlItem(node, this);
    m_childItems.append(childItem);
    return true;
}

QVariant TagXmlItem::value(const QString& attr)
{
    QDomNamedNodeMap attributeMap = m_domNode.attributes();
    QString value = attributeMap.namedItem(attr).nodeValue();

    if (value == QString("true"))
        return QVariant(true);

    if (value == QString("false"))
        return QVariant(false);

    return value;
}

void TagXmlItem::setValue(const QString& attr, const QVariant &val)
{
    m_domNode.toElement().setAttribute(attr,val.toString());
}

bool TagXmlItem::insertChild(const QString& tagName, int i, QStringList tags, QStringList parentTags)
{
    TagXmlItem *afterItem = child(i, tags, parentTags);
    if (!afterItem) return insertChild(tagName);

    TagXmlItem *parentItem = afterItem->parent();
    if (!parentItem) return false;

    if (this->node() != afterItem->node().parentNode())
        return insertChild(tagName);

    QDomElement node = m_domNode.ownerDocument().createElement(tagName);

    if  (node.isNull())
        return false;

    m_domNode.insertBefore(node, afterItem->node());
    TagXmlItem *childItem = new TagXmlItem(node, this);
    m_childItems.insert(parentItem->m_childItems.indexOf(afterItem), childItem);
    return true;
}

bool TagXmlItem::removeChild(const int &row)
{ 
    TagXmlItem *childItem = child(row);
    m_domNode.removeChild(childItem->node());
    delete m_childItems.takeAt(row);

    return true;
}

bool TagXmlItem::checkRemoveChild(const int &row){

    if (m_childItems.count() > row)
            return true;

    return false;
}
