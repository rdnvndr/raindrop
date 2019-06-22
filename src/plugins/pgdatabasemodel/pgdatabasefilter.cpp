#include "pgdatabasefilter.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseFilter::~PgDatabaseFilter()
{

}

IDatabaseError PgDatabaseFilter::push(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseFilter::pull(IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseFilter::rename(const QString &name,
                                                   IDatabaseSession *session)
{
    return nullptr;
}

IDatabaseError PgDatabaseFilter::remove(IDatabaseSession *session)
{
    return nullptr;
}

}}
