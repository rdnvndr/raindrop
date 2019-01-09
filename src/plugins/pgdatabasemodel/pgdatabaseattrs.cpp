#include "pgdatabaseattrs.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseAttrs::PgDatabaseAttrs(PgDatabaseClass *item)
    :IDatabaseList<PgDatabaseClass, PgDatabaseAttribute>(item)
{

}

PgDatabaseAttrs::~PgDatabaseAttrs()
{

}

PgDatabaseAttribute *PgDatabaseAttrs::value()
{
    return nullptr;
}



}}
