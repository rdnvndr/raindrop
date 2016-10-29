#include "pgdatabasecomposition.h"

PgDatabaseComposition::PgDatabaseComposition()
{

}

PgDatabaseComposition::~PgDatabaseComposition()
{

}

QUuid PgDatabaseComposition::id()
{
    return QUuid();
}

void PgDatabaseComposition::setId(QUuid id)
{

}

QString PgDatabaseComposition::name()
{
    return QString();
}

void PgDatabaseComposition::setName(const QString &name)
{

}

QString PgDatabaseComposition::alias()
{
    return QString();
}

void PgDatabaseComposition::setAlias(const QString &alias)
{

}

bool PgDatabaseComposition::isExist()
{
    return true;
}

bool PgDatabaseComposition::push()
{
    return true;
}

bool PgDatabaseComposition::pull()
{
    return true;
}

bool PgDatabaseComposition::remove()
{
    return true;
}
