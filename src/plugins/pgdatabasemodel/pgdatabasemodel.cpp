#include "pgdatabasemodel.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

IDatabaseClass *PgDatabaseModel::createModelClass(const QString &name)
{
    Q_UNUSED(name)

    return NULL;
}

IDatabaseClass *PgDatabaseModel::modelClass(const QString &name)
{
    Q_UNUSED(name)
    return NULL;
}

IDatabaseClasses *PgDatabaseModel::classList()
{
    return NULL;
}

IDatabaseClasses *PgDatabaseModel::modelClassList()
{
    return NULL;
}

}}
