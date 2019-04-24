#ifndef PGDATABASEATTRS_H
#define PGDATABASEATTRS_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"
#include "pgdatabaseattribute.h"
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация списка атрибутов класса базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseAttrs
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
