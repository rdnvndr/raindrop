#ifndef PGDATABASEFILTER_H
#define PGDATABASEFILTER_H

#include <databasemodel/idatabasefilter.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация фильтра базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseFilter : public IDatabaseFilter
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseFilter();

    //! Отправить изменения элемента базы данных
    void push(IDatabaseSession *session = nullptr);

    //! Получить изменения элемента базы данных
    void pull(IDatabaseSession *session = nullptr);

    //! Переименовывает элемент базы данных
    virtual void rename(const QString &name, IDatabaseSession *session = nullptr);

    //! Удаление элемента базы данных
    void remove(IDatabaseSession *session = nullptr);
};

}}

#endif // PGDATABASEFILTER_H
