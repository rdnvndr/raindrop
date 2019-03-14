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

void PgDatabaseClass::create(IDatabaseSession *session)
{
    ThreadQuery *query = (session) ? m_pool->acquire(session->id())
                                          : m_pool->acquire();

    QUuid uuidOper = QUuid::createUuid();
    auto uuidRoll = std::make_shared<QUuid>();
    *uuidRoll = QUuid::createUuid();

    auto connErr  = std::make_shared<QMetaObject::Connection>();
    auto connDone = std::make_shared<QMetaObject::Connection>();

    // Обработка ошибки
    *connErr = QObject::connect(query, &ThreadQuery::error, query,
                   [this, query, session, uuidOper, uuidRoll, connErr, connDone]
                   (const QUuid &queryUuid, const QSqlError &err)
    {
        if (!uuidRoll->isNull() && *uuidRoll != queryUuid) {
            QObject::disconnect(*connErr);
            QObject::disconnect(*connDone);
            *uuidRoll = QUuid();
            if (session == nullptr) {
                query->rollback();
                query->end();
                delete query;
            } else {
                query->execute("ROLLBACK TO SAVEPOINT "
                               + lblUuidPoint(uuidOper) + ";");
                query->execute("RELEASE SAVEPOINT "
                               + lblUuidPoint(uuidOper) + ";");
                query->end();
            }
            emit this->error(err);
        }
    }, Qt::QueuedConnection);

    // Окончание запроса
    *connDone = QObject::connect(query, &ThreadQuery::executeDone, query,
                     [this, query, session, uuidOper, connErr, connDone]
                     (const QUuid &queryUuid)
    {
        if (uuidOper != queryUuid)
            return;

        QObject::disconnect(*connErr);
        QObject::disconnect(*connDone);

        if (session == nullptr) {
            query->end();
            query->commit();
            delete query;
        } else {
            query->execute("RELEASE SAVEPOINT " + lblUuidPoint(uuidOper) + ";");
            query->end();
        }
        emit this->done();
    }, Qt::QueuedConnection);

    query->begin();
    if (session == nullptr)
        query->transaction();
    else {
        query->execute("SAVEPOINT " + lblUuidPoint(uuidOper) + ";", *uuidRoll);
    }

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
}

void PgDatabaseClass::push(IDatabaseSession *session)
{

}

void PgDatabaseClass::pull(IDatabaseSession *session)
{

}

void PgDatabaseClass::remove(IDatabaseSession *session)
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
