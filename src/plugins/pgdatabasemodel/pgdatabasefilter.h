#ifndef PGDATABASEFILTER_H
#define PGDATABASEFILTER_H

#include <databasemodel/idatabasefilter.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация фильтра базы данных PostgreSql
class PgDatabaseFilter : public IDatabaseFilter
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseFilter();

    //! Отправить изменения элемента базы данных
    void push(IDatabaseThread *databaseThread = nullptr);

    //! Получить изменения элемента базы данных
    void pull(IDatabaseThread *databaseThread = nullptr);

    //! Удаление элемента базы данных
    void remove(IDatabaseThread *databaseThread = nullptr);
};

}}

#endif // PGDATABASEFILTER_H
