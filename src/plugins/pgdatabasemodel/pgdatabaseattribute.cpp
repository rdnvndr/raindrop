#include "pgdatabaseattribute.h"

#include <QVariant>

#include "pgdatabaseexpression.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseAttribute::~PgDatabaseAttribute()
{

}

void PgDatabaseAttribute::create(IDatabaseSession *session)
{

}

void PgDatabaseAttribute::push(IDatabaseSession *session)
{

}

void PgDatabaseAttribute::pull(IDatabaseSession *session)
{

}

void PgDatabaseAttribute::rename(const QString &name, IDatabaseSession *session)
{

}

void PgDatabaseAttribute::remove(IDatabaseSession *session)
{

}

bool PgDatabaseAttribute::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}

}}
