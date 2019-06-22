#include "pgdatabasecomposition.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseComposition::~PgDatabaseComposition()
{

}

IDatabaseError PgDatabaseComposition::create(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseComposition::push(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseComposition::pull(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseComposition::rename(const QString &name,
                                                        IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseComposition::remove(IDatabaseSession *session)
{
    return nullptr;
}

}}
