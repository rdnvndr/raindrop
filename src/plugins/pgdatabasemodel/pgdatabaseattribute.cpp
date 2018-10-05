#include "pgdatabaseattribute.h"

#include <QVariant>

#include "pgdatabaseexpression.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseAttribute::~PgDatabaseAttribute()
{

}

void PgDatabaseAttribute::create(IDatabaseThread *databaseThread)
{

}

void PgDatabaseAttribute::push(IDatabaseThread *databaseThread)
{

}

void PgDatabaseAttribute::pull(IDatabaseThread *databaseThread)
{

}

void PgDatabaseAttribute::remove(IDatabaseThread *databaseThread)
{

}

bool PgDatabaseAttribute::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}

}}
