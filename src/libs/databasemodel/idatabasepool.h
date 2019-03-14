#ifndef IDATABASEPOOL_H
#define IDATABASEPOOL_H

#include <databasemodel/idatabasepool.h>
#include <sqlextension/threadquery.h>
#include <sqlextension/threadquerypool.h>

#include <QUuid>
#include <QSqlDatabase>

using namespace RTPTechGroup::SqlExtension;

//! Модель пул базы данных
class IDatabasePool
{

public:
    //! Конструктор класса
    IDatabasePool(const QSqlDatabase &db);

    //! Деструктор класса
    virtual ~IDatabasePool();

    //! Резервирует sql запрос
    ThreadQuery *acquire(QUuid sessionId = QUuid());

    //! Возвращает sql запрос потока
    void release(QUuid sessionId = QUuid());

private:
    //! Список запросов
    QHash<QUuid, ThreadQueryItem<ThreadQuery> *> m_threadQuery;

    //! Пул многопоточных Sql запросов
    ThreadQueryPool<ThreadQuery> *m_queryPool;
};



#endif // IDATABASEPOOL_H
