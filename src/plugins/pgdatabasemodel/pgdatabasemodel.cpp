#include "pgdatabasemodel.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

}}
