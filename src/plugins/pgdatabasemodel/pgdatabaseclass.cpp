#include "pgdatabaseclass.h"

#include "pgdatabaseclasses.h"
#include "pgdatabaseattrs.h"
#include "pgdatabasefilters.h"
#include "pgdatabasecomps.h"
#include "pgdatabaseobjects.h"

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

IDatabaseClass *PgDatabaseClass::derivedClass(const QString &name)
{
    Q_UNUSED(name)
    return NULL;
}

IDatabaseAttribute *PgDatabaseClass::attr(const QString &name)
{
    Q_UNUSED(name)
    return NULL;
}

IDatabaseFilter *PgDatabaseClass::filter(const QString &name)
{
    Q_UNUSED(name)
    return NULL;
}

IDatabaseComposition *PgDatabaseClass::comp(const QString &name)
{
    Q_UNUSED(name)
    return NULL;
}

IDatabaseClasses *PgDatabaseClass::derivedClassList()
{
    return dynamic_cast<IDatabaseClasses *>(new PgDatabaseClasses(this));
}

IDatabaseAttrs *PgDatabaseClass::attrList()
{
    return dynamic_cast<IDatabaseAttrs *>(new PgDatabaseAttrs(this));
}

IDatabaseFilters *PgDatabaseClass::filterList()
{
    return dynamic_cast<IDatabaseFilters *>(new PgDatabaseFilters(this));
}

IDatabaseComps *PgDatabaseClass::compList()
{
    return dynamic_cast<IDatabaseComps *>(new PgDatabaseComps(this));
}

IDatabaseObjects *PgDatabaseClass::filtering(IDatabaseFilter *filter)
{
    Q_UNUSED(filter)

    return NULL;
}

IDatabaseObjects *PgDatabaseClass::filtering(IDatabaseExpression *expr)
{
    Q_UNUSED(expr)

    return NULL;
}

IDatabaseObjects *PgDatabaseClass::all()
{
    return dynamic_cast<IDatabaseObjects *>(new PgDatabaseObjects(this));
}

}}
