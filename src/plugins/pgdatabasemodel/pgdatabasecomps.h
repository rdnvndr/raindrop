#ifndef PGDATABASECOMPS_H
#define PGDATABASECOMPS_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"
#include "pgdatabasecomposition.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация списка составов базы данных PostgreSql
class PgDatabaseComps
        : public IDatabaseList<PgDatabaseClass, PgDatabaseComposition>
{
public:
    //! Конструктор класса
    explicit PgDatabaseComps(PgDatabaseClass *item);

    //! Деструктор класса
    virtual ~PgDatabaseComps();

    //! Получение состава базы данных
    PgDatabaseComposition *value();
};

}}

#endif // PGDATABASECOMPS_H
