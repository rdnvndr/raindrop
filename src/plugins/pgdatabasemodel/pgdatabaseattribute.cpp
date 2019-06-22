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
    return nullptr;
}

IDatabaseError PgDatabaseAttribute::push(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseAttribute::pull(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseAttribute::rename(const QString &name,
                                                      IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseAttribute::remove(IDatabaseSession *session)
{
    return nullptr;
}

bool PgDatabaseAttribute::isInherited(IDatabaseClass *parent)
{
    Q_UNUSED(parent)
    return true;
}

}}
