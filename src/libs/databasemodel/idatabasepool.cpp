#include "idatabasepool.h"

IDatabasePool::IDatabasePool(const QSqlDatabase &db)
{
    m_queryPool = new ThreadQueryPool<ThreadQuery>(db);
}

IDatabasePool::~IDatabasePool()
{
    delete m_queryPool;
}

ThreadQuery *IDatabasePool::acquire(QUuid sessionId)
{
    if (!sessionId.isNull()) {
        ThreadQueryItem<ThreadQuery> *threadQuery = m_threadQuery.value(sessionId);
        if (threadQuery == nullptr) {
            threadQuery = m_queryPool->acquire();
        }

        m_threadQuery.insert(sessionId, threadQuery);
        return threadQuery;
    }

    return m_queryPool->acquire();
}

void IDatabasePool::release(QUuid sessionId)
{
    if (!sessionId.isNull()) {
        ThreadQueryItem<ThreadQuery> *threadQuery = m_threadQuery.value(sessionId);
        if (threadQuery) {
            m_queryPool->remove(threadQuery);
        }
    }
}
