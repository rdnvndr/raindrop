#ifndef PGDATABASEFILTERS_H
#define PGDATABASEFILTERS_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"
#include "pgdatabasefilter.h"
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация списка фильтров базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseFilters
        : public IDatabaseList<PgDatabaseClass, PgDatabaseFilter>
{
public:
    //! Конструктор класса
    explicit PgDatabaseFilters(PgDatabaseClass *item);

    //! Деструктор класса
    virtual ~PgDatabaseFilters();

    //! Получение фильтра базы данных
    PgDatabaseFilter *value();
};

}}

#endif // PGDATABASEFILTERS_H
