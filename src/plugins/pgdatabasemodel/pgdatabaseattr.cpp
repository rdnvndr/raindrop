#include "pgdatabaseattr.h"

#include "pgdatabaseexpression.h"

#include <QDebug>
#include <QVariant>

PgDatabaseAttr::PgDatabaseAttr()
{

}

PgDatabaseAttr::~PgDatabaseAttr()
{

}

bool PgDatabaseAttr::isExist()
{
    return true;
}

bool PgDatabaseAttr::push()
{
    return true;
}

bool PgDatabaseAttr::pull()
{
    return true;
}

bool PgDatabaseAttr::remove()
{
    return true;
}

bool PgDatabaseAttr::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}


