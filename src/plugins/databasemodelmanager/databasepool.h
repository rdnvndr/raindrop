#ifndef DATABASEPOOL_H
#define DATABASEPOOL_H

#include "databasemodelmanagerglobal.h"

#include <idatabasepool.h>
#include <sqlextension/threadquery.h>
#include <sqlextension/threadquerypool.h>

#include <QUuid>
#include <QSqlDatabase>

using namespace RTPTechGroup::SqlExtension;

namespace RTPTechGroup {
namespace DatabaseModel {

class DATABASEMODELMANAGERLIB DatabasePool : public IDatabasePool
{
public:
    //! Конструктор класса
    explicit DatabasePool(const QSqlDatabase &db);

    //! Деструктор класса
    virtual ~DatabasePool();

    //! Получение sql запроса потока
    virtual SqlExtension::ThreadQuery *acquire(QUuid threadUuid = QUuid());

    //! Возвращает sql запрос потока
    virtual void release(QUuid threadUuid = QUuid());

private:
    //! Список запросов
    QHash<QUuid, ThreadQueryItem<ThreadQuery> *> m_threadQuery;

    //! Пул многопоточных Sql запросов
    ThreadQueryPool<ThreadQuery> *m_queryPool;
};

}}

#endif // DATABASEPOOL_H
