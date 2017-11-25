#include "pgdatabasemodel.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

bool PgDatabaseModel::init()
{
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
