#include "pgdatabasecomps.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseComps::PgDatabaseComps(PgDatabaseClass *item)
    :IDatabaseList<PgDatabaseClass, PgDatabaseComposition>(item)
{

}

PgDatabaseComps::~PgDatabaseComps()
{

}

PgDatabaseComposition *PgDatabaseComps::value()
{
    return NULL;
}

}}
