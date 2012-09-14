#include "objectcollection.h"

ObjectCollection::ObjectCollection(QObject *parent) :
    QObject(parent)
{
    m_objects.clear();
}

void ObjectCollection::addObject(QPointer<QObject> obj)
{
    m_objects.insert(obj->objectName(), obj);
}

void ObjectCollection::removeObject(QPointer<QObject> obj)
{
    m_objects.remove(obj->objectName());
}

QPointer<QObject> ObjectCollection::getObjectByName(const QString &name) const
{
    return m_objects[name];
}

QPointer<QObject> ObjectCollection::getObjectByClassName(const QString &className) const
{
    const QByteArray baClassName = className.toUtf8();
    QHashIterator<QString, QObject *> obj(m_objects);
    while (obj.hasNext())
    {
        obj.next();
        if (obj.value()->inherits(baClassName.constData()))
        {
            return obj.value();
        }
    }

    return NULL;
}
