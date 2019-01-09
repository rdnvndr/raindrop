#include "pgdatabasefilters.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseFilters::PgDatabaseFilters(PgDatabaseClass *item)
    :IDatabaseList<PgDatabaseClass, PgDatabaseFilter>(item)
{

}

PgDatabaseFilters::~PgDatabaseFilters()
{

}

PgDatabaseFilter *PgDatabaseFilters::value()
{
    return nullptr;
}

}}
