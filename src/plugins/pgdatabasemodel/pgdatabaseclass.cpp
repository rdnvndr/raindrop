#include "pgdatabaseclass.h"

#include <QtCore/QObject>

#include <sqlextension/threadquery.h>
#include <metadatamodel/dbxmlstruct.h>

#include "pgdatabaseclasses.h"
#include "pgdatabaseattrs.h"
#include "pgdatabasefilters.h"
#include "pgdatabasecomps.h"
#include "pgdatabaseobjects.h"
#include "pgdatabasehelper.h"

using namespace RTPTechGroup::SqlExtension;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseClass::PgDatabaseClass(const QString &name, IDatabasePool *pool)
    : IDatabaseClass (name, pool)
{

}

PgDatabaseClass::~PgDatabaseClass()
{

}

IDatabaseError PgDatabaseClass::create(IDatabaseSession *session)
{
    IDatabaseError error(session);QUuid uuidOper = QUuid::createUuid();
    ThreadQuery *query = autoDoneQuery(uuidOper, error, m_pool);



    query->execute("CREATE TABLE " + clsTable(this->name()) + "();");
    query->prepare(
        "INSERT INTO " + clsTable(DBCLASSXML::CLASS) + " ("
                  + tblField(DBCLASSXML::NAME)     + ","
                  + tblField(DBCLASSXML::MODE)     + ","
                  + tblField(DBCLASSXML::TYPE)     + ","
                  + tblField(DBCLASSXML::ALIAS)    + ","
                  + tblField(DBCLASSXML::PARENT)   + ","
                  + tblField(DBCLASSXML::TEMPLATE) + ","
                  + tblField(DBCLASSXML::VERCOUNT) + ","
                  + tblField(DBCLASSXML::ICON)     + ","
                  + tblField(DBCLASSXML::ID)
        + ") VALUES("
              ":name, :mode, :type, :alias, :parent, :template, "
              ":vercount, :icon, :id"
          ");"
    );

    if (this->id().isNull())
        this->setId(QUuid::createUuid());
    IDatabaseClass *baseClass = this->baseClass();
    query->bindValue(":name",     this->name());
    query->bindValue(":mode",     vlAccessMode(this->accessMode()));
    query->bindValue(":type",     vlClassType(this->classType()));
    query->bindValue(":alias",    this->alias());
    query->bindValue(":parent",   (baseClass) ? vlUuidString(baseClass->id())
                                              : QString());
    query->bindValue(":template", this->objectNameTemplate());
    query->bindValue(":vercount", this->maxVersion());
    query->bindValue(":icon",     this->icon());
    query->bindValue(":id",       vlUuidString(this->id()));
    query->execute(uuidOper);

    return error;
}

IDatabaseError PgDatabaseClass::push(IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseClass::pull(IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseClass::rename(const QString &name,
                                                  IDatabaseSession *session)
{

}

IDatabaseError PgDatabaseClass::remove(IDatabaseSession *session)
{

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
