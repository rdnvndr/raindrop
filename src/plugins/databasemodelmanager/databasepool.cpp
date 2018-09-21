#include "databasepool.h"

using namespace RTPTechGroup::SqlExtension;

namespace RTPTechGroup {
namespace DatabaseModel {

DatabasePool::DatabasePool(const QSqlDatabase &db)
{
    m_db = db;
}

ThreadQuery *DatabasePool::acquire(QUuid threadUuid)
{
    return new ThreadQuery(m_db);
}

ThreadQuery *DatabasePool::find(QUuid threadUuid)
{
    return nullptr;
}

void DatabasePool::release(QUuid threadUuid)
{

}

}}
