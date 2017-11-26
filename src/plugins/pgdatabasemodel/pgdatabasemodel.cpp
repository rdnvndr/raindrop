#include "pgdatabasemodel.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

bool PgDatabaseModel::init()
{
    m_db.transaction();
    QSqlQuery query = QSqlQuery(m_db);

    query.exec("CREATE TABLE \"CLASS\" ("
               "    \"NAME\"            VARCHAR(27),"
               "    \"ACCESSMODE\"      CHAR(36),"
               "    \"TYPE\"            CHAR(36),"
               "    \"ALIAS\"           VARCHAR(256),"
               "    \"PARENT\"          CHAR(36),"
               "    \"DISPLAYTEMPLATE\" VARCHAR(4000),"
               "    \"VERSIONCOUNT\"    INTEGER,"
               "    \"ICON\"            CHAR(36),"
               "    \"GUID\"            CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"ATTRIBUTE\" ("
               "    \"NAME\"                VARCHAR(27),"
               "    \"ALIAS\"               VARCHAR(256),"
               "    \"TYPE\"                VARCHAR(15),"
               "    \"GROUP\"               VARCHAR(256),"
               "    \"MAXSTRLEN\"           INTEGER,"
               "    \"ACCURACY\"            INTEGER,"
               "    \"ISNULLSALLOWED\"      BOOL,"
               "    \"ISUNIQUE\"            BOOL,"
               "    \"ISCANDIDATEKEY\"      BOOL,"
               "    \"INITIALVALUE\"        VARCHAR(4000),"
               "    \"LOWERBOUND\"          VARCHAR(4000),"
               "    \"UPPERBOUND\"          VARCHAR(4000),"
               "    \"REFERENCEDCLASS\"     CHAR(36),"
               "    \"REFERENCEDUNIT\"      CHAR(36),"
               "    \"REFERENCEDLOV\"       CHAR(36),"
               "    \"REFERENCENUMERATOR\"  CHAR(36),"
               "    \"PARENT\"              CHAR(36),"
               "    \"GUID\"                CHAR(36)"
               ");");

    m_db.commit();
    return true;
}

IDatabaseClass *PgDatabaseModel::createDerivedClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return NULL;
}

IDatabaseClass *PgDatabaseModel::derivedClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return NULL;
}

IDatabaseClasses *PgDatabaseModel::allClassList()
{
    return NULL;
}

IDatabaseClasses *PgDatabaseModel::derivedClassList(const QString &base)
{
    Q_UNUSED(base)

    return NULL;
}

}}
