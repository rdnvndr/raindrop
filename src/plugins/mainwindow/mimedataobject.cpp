#include "mimedataobject.h"

MimeDataObject::MimeDataObject(): QMimeData ()
{

}

void MimeDataObject::setObject(QAction *widget)
{
    m_object = widget;
}

QObject *MimeDataObject::object() const
{
    return m_object;
}

bool MimeDataObject::hasObject() const
{
    return (m_object)?true:false;
}
