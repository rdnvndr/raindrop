#include "mimedataobject.h"

namespace RTPTechGroup {
namespace MainWindow {

MimeDataObject::MimeDataObject(): QMimeData ()
{
    m_object = NULL;
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

QStringList MimeDataObject::formats() const
{
    QStringList format = QMimeData::formats();
    if (hasObject()) format << "application/x-qobject";
    return format;
}

}
}
