#ifndef DATABASEPOOL_H
#define DATABASEPOOL_H

#include "databasemodelmanagerglobal.h"

#include <idatabasepool.h>
#include <sqlextension/threadquery.h>

#include <QUuid>
#include <QSqlDatabase>

namespace RTPTechGroup {
namespace DatabaseModel {

class DATABASEMODELMANAGERLIB DatabasePool : public IDatabasePool
{
public:
    //! Конструктор класса
    explicit DatabasePool(const QSqlDatabase &db);

    //! Резервирует sql запрос потока
    virtual SqlExtension::ThreadQuery *acquire(QUuid threadUuid);

    //! Получение sql запроса потока
    virtual RTPTechGroup::SqlExtension::ThreadQuery *find(QUuid threadUuid);

    //! Возвращает sql запрос потока
    virtual void release(QUuid threadUuid);

private:
    QSqlDatabase m_db;
};

}}

#endif // DATABASEPOOL_H