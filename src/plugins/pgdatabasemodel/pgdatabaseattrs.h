#ifndef PGDATABASEATTRS_H
#define PGDATABASEATTRS_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"
#include "pgdatabaseattribute.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация списка атрибутов класса базы данных PostgreSql
class PgDatabaseAttrs
        : public IDatabaseList<PgDatabaseClass, PgDatabaseAttribute>
{
public:
    //! Конструктор класса
    explicit PgDatabaseAttrs(PgDatabaseClass *item);

    //! Деструктор класса
    virtual ~PgDatabaseAttrs();

    //! Получение атрибута базы данных
    PgDatabaseAttribute *value();
};

}}

#endif // PGDATABASEATTRS_H
