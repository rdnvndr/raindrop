#include "pgdatabaseobjects.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseObjects::PgDatabaseObjects(PgDatabaseClass *item)
 :IDatabaseList<PgDatabaseClass, PgDatabaseObject>(item)
{

}

PgDatabaseObjects::~PgDatabaseObjects()
{

}

PgDatabaseObject *PgDatabaseObjects::value()
{
    return NULL;
}

}}
