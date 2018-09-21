#include "databasepool.h"

using namespace RTPTechGroup::SqlExtension;

namespace RTPTechGroup {
namespace DatabaseModel {

DatabasePool::DatabasePool(const QSqlDatabase &db)
{

}

ThreadQuery *DatabasePool::acquire(QUuid threadUuid)
{
    return nullptr;
}

ThreadQuery *DatabasePool::find(QUuid threadUuid)
{
    return nullptr;
}

void DatabasePool::release(QUuid threadUuid)
{

}

}}
