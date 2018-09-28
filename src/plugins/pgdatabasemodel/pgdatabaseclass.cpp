#include "pgdatabaseclass.h"

#include "pgdatabaseclasses.h"
#include "pgdatabaseattrs.h"
#include "pgdatabasefilters.h"
#include "pgdatabasecomps.h"
#include "pgdatabaseobjects.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseClass::PgDatabaseClass(const QString &name, IDatabasePool *pool)
    : IDatabaseClass (name, pool)
{

}

PgDatabaseClass::~PgDatabaseClass()
{

}

void PgDatabaseClass::create()
{

}

void PgDatabaseClass::push()
{

}

void PgDatabaseClass::pull()
{

}

void PgDatabaseClass::remove()
{

}

IDatabaseClass *PgDatabaseClass::createClass(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseAttribute *PgDatabaseClass::createAttr(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseFilter *PgDatabaseClass::createFilter(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseComposition *PgDatabaseClass::createComp(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseClass *PgDatabaseClass::derivedClass(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseAttribute *PgDatabaseClass::attr(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseFilter *PgDatabaseClass::filter(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
}

IDatabaseComposition *PgDatabaseClass::comp(const QString &name)
{
    Q_UNUSED(name)
    return nullptr;
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

    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::filtering(IDatabaseExpression *expr)
{
    Q_UNUSED(expr)

    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::orderByAsc(IDatabaseAttribute *attr, ...)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::orderByDesc(IDatabaseAttribute *attr, ...)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::distinct(IDatabaseAttribute *attr, ...)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::limit(int up, int down)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::values(IDatabaseAttribute *attr, ...)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::join(IDatabaseFilter *filter, IDatabaseManipulation::JoinType joinType)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::join(IDatabaseExpression *expr, IDatabaseManipulation::JoinType joinType)
{
    return nullptr;
}

IDatabaseObjects *PgDatabaseClass::unionAll(IDatabaseObjects *objs)
{
    return nullptr;
}

int PgDatabaseClass::count(IDatabaseAttribute *attr, bool distinct)
{
    return 0;
}

int PgDatabaseClass::avg(IDatabaseAttribute *attr)
{
    return 0;
}

int PgDatabaseClass::min(IDatabaseAttribute *attr)
{
    return 0;
}

int PgDatabaseClass::max(IDatabaseAttribute *attr)
{
    return 0;
}

int PgDatabaseClass::sum(IDatabaseAttribute *attr)
{
    return 0;
}

/*
IDatabaseObjects *PgDatabaseClass::all()
{
    return nullptr;
    //return dynamic_cast<IDatabaseObjects *>(new PgDatabaseObjects(this));
}
*/
}}
