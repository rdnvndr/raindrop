#include "databasepool.h"

namespace RTPTechGroup {
namespace DatabaseModel {

DatabasePool::DatabasePool(const QSqlDatabase &db)
{
    m_queryPool = new ThreadQueryPool<ThreadQuery>(db);
}

DatabasePool::~DatabasePool()
{
    delete m_queryPool;
}

ThreadQuery *DatabasePool::acquire(QUuid threadUuid)
{
    if (!threadUuid.isNull()) {
        ThreadQueryItem<ThreadQuery> *threadQuery = m_threadQuery.value(threadUuid);
        if (threadQuery == nullptr) {
            threadQuery = m_queryPool->acquire();
        }

        m_threadQuery.insert(threadUuid, threadQuery);
        return threadQuery;
    }

    return m_queryPool->acquire();
}

void DatabasePool::release(QUuid threadUuid)
{
    if (!threadUuid.isNull()) {
        ThreadQueryItem<ThreadQuery> *threadQuery = m_threadQuery.value(threadUuid);
        if (threadQuery) {
            m_queryPool->remove(threadQuery);
        }
    }
}

}}
