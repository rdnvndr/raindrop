#include "pgdatabaseclass.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseClass::PgDatabaseClass()
{

}

PgDatabaseClass::~PgDatabaseClass()
{

}

bool PgDatabaseClass::isExist()
{
    return  true;
}

bool PgDatabaseClass::push()
{
    return  true;
}

bool PgDatabaseClass::pull()
{
    return  true;
}

bool PgDatabaseClass::remove()
{
    return  true;
}

IDatabaseAttrs PgDatabaseClass::attrList()
{
    return IDatabaseAttrs();
}

IDatabaseFilters PgDatabaseClass::filterList()
{
    return IDatabaseFilters();
}

void PgDatabaseClass::filtering(IDatabaseFilter *filter)
{
    Q_UNUSED(filter)
}

void PgDatabaseClass::filtering(IDatabaseExpression *expr)
{
    Q_UNUSED(expr)
}

void PgDatabaseClass::all()
{

}

}}
