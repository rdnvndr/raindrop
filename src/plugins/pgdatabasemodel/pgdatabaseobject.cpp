#include "pgdatabaseobject.h"

namespace RTPTechGroup {
namespace DatabaseModel {


PgDatabaseObject::~PgDatabaseObject()
{

}

IDatabaseError PgDatabaseObject::create(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseObject::push(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseObject::pull(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseObject::rename(const QString &name, IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseObject::remove(IDatabaseSession *session)
{
    return nullptr;
}

}}
