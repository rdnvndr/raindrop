#include "idatabasepool.h"

IDatabasePool::IDatabasePool(const QSqlDatabase &db)
{
    m_queryPool = new ThreadQueryPool<ThreadQuery>(db);
}

IDatabasePool::~IDatabasePool()
{
    delete m_queryPool;
}

ThreadQuery *IDatabasePool::acquire(QUuid threadUuid)
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

void IDatabasePool::release(QUuid threadUuid)
{
    if (!threadUuid.isNull()) {
        ThreadQueryItem<ThreadQuery> *threadQuery = m_threadQuery.value(threadUuid);
        if (threadQuery) {
            m_queryPool->remove(threadQuery);
        }
    }
}
