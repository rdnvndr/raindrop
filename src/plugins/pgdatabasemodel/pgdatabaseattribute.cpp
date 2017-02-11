#include "pgdatabaseattribute.h"

#include <QVariant>

#include "pgdatabaseexpression.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseAttribute::PgDatabaseAttribute()
{

}

PgDatabaseAttribute::~PgDatabaseAttribute()
{

}

bool PgDatabaseAttribute::isExist()
{
    return true;
}

bool PgDatabaseAttribute::push()
{
    return true;
}

bool PgDatabaseAttribute::pull()
{
    return true;
}

bool PgDatabaseAttribute::remove()
{
    return true;
}

bool PgDatabaseAttribute::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}

}}
