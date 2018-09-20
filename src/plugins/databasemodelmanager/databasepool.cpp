#include "databasepool.h"

namespace RTPTechGroup {
namespace DatabaseModel {

DatabasePool::DatabasePool(const QSqlDatabase &db)
{

}

void DatabasePool::acquire(QUuid threadUuid)
{

}

RTPTechGroup::SqlExtension::ThreadQuery *DatabasePool::threadQuery(QUuid threadUuid)
{
    return nullptr;
}

void DatabasePool::release(QUuid threadUuid)
{

}

}}
