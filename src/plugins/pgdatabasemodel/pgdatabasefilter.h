#ifndef PGDATABASEFILTER_H
#define PGDATABASEFILTER_H

#include <idatabasefilter.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация фильтра базы данных PostgreSql
class PgDatabaseFilter : public IDatabaseFilter
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseFilter();

    //! Отправить изменения элемента базы данных
    void push();

    //! Получить изменения элемента базы данных
    void pull();

    //! Удаление элемента базы данных
    void remove();
};

}}

#endif // PGDATABASEFILTER_H
