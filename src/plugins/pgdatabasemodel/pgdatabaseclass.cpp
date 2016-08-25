#include "pgdatabaseclass.h"

PgDatabaseClass::PgDatabaseClass()
{

}

QUuid PgDatabaseClass::id()
{
    return QUuid();
}

void PgDatabaseClass::setId(QUuid id)
{

}

QIcon PgDatabaseClass::icon()
{
    return QIcon();
}

void PgDatabaseClass::setIcon(QIcon icon)
{

}

QString PgDatabaseClass::name()
{
    return QString("");
}

void PgDatabaseClass::setName(const QString &name)
{

}

QString PgDatabaseClass::alias()
{
    return QString("");
}

void PgDatabaseClass::setAlias(const QString &alias)
{

}

IDatabaseClass::ClassType PgDatabaseClass::classType()
{
    return ClassType::Normal;
}

void PgDatabaseClass::setClassType(IDatabaseClass::ClassType classType)
{

}

IDatabaseClass::AccessMode PgDatabaseClass::accessMode()
{
    return AccessMode::Standart;
}

void PgDatabaseClass::setAccessMode(IDatabaseClass::AccessMode accessMode)
{

}

IDatabaseClass *PgDatabaseClass::parent()
{
    return NULL;
}

void PgDatabaseClass::setParent(IDatabaseClass *parent)
{

}

QString PgDatabaseClass::objectNameTemplate()
{
    return QString("");
}

void PgDatabaseClass::setObjectNameTemplate(const QString &objectNameTemplate)
{

}

qint32 PgDatabaseClass::maxVersion()
{
    return 0;
}

void PgDatabaseClass::setMaxVersion(qint32 maxVersion)
{

}

IDatabaseAttrList PgDatabaseClass::attrList()
{
    return IDatabaseAttrList();
}
