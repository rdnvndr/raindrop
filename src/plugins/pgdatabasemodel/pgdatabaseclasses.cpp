#include "pgdatabaseclasses.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseClasses::PgDatabaseClasses(PgDatabaseClass *item)
    :IDatabaseList<PgDatabaseClass, PgDatabaseClass>(item)
{

}

PgDatabaseClasses::~PgDatabaseClasses()
{

}

PgDatabaseClass *PgDatabaseClasses::value()
{
    return NULL;
}

}}
