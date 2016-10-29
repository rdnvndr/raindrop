#include "pgdatabaseclass.h"

PgDatabaseClass::PgDatabaseClass()
{

}

PgDatabaseClass::~PgDatabaseClass()
{

}

QUuid PgDatabaseClass::id()
{
    return QUuid();
}

void PgDatabaseClass::setId(QUuid id)
{
    Q_UNUSED(id)

}

QIcon PgDatabaseClass::icon()
{
    return QIcon();
}

void PgDatabaseClass::setIcon(QIcon icon)
{
    Q_UNUSED(icon)
}

QString PgDatabaseClass::name()
{
    return QString("");
}

void PgDatabaseClass::setName(const QString &name)
{
    Q_UNUSED(name)
}

QString PgDatabaseClass::alias()
{
    return QString("");
}

void PgDatabaseClass::setAlias(const QString &alias)
{
    Q_UNUSED(alias)
}

IDatabaseClass::ClassType PgDatabaseClass::classType()
{
    return ClassType::Normal;
}

void PgDatabaseClass::setClassType(IDatabaseClass::ClassType classType)
{
    Q_UNUSED(classType)
}

IDatabaseClass::AccessMode PgDatabaseClass::accessMode()
{
    return AccessMode::Standart;
}

void PgDatabaseClass::setAccessMode(IDatabaseClass::AccessMode accessMode)
{
    Q_UNUSED(accessMode)
}

IDatabaseClass *PgDatabaseClass::parent()
{
    return NULL;
}

void PgDatabaseClass::setParent(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
}

QString PgDatabaseClass::objectNameTemplate()
{
    return QString("");
}

void PgDatabaseClass::setObjectNameTemplate(const QString &objectNameTemplate)
{
    Q_UNUSED(objectNameTemplate)
}

qint32 PgDatabaseClass::maxVersion()
{
    return 0;
}

void PgDatabaseClass::setMaxVersion(qint32 maxVersion)
{
    Q_UNUSED(maxVersion)
}

IDatabaseAttrs PgDatabaseClass::attrList()
{
    return IDatabaseAttrs();
}

IDatabaseFilters PgDatabaseClass::filterList()
{
    return IDatabaseFilters();
}

void PgDatabaseClass::filtering(IDatabaseFilter *filter)
{
    Q_UNUSED(filter)
}

void PgDatabaseClass::filtering(IDatabaseExpression *expr)
{
    Q_UNUSED(expr)
}

void PgDatabaseClass::all()
{

}
