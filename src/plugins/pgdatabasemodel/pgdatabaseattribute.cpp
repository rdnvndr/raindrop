#include "pgdatabaseattribute.h"

#include <QVariant>

#include "pgdatabaseexpression.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseAttribute::~PgDatabaseAttribute()
{

}

IDatabaseError PgDatabaseAttribute::create(IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseAttribute::push(IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseAttribute::pull(IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseAttribute::rename(const QString &name,
                                                      IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseAttribute::remove(IDatabaseSession *session)
{

}

bool PgDatabaseAttribute::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}

}}
