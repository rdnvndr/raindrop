#ifndef PGDATABASECLASSES_H
#define PGDATABASECLASSES_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"

namespace RTPTechGroup {
namespace DatabaseModel {

class PgDatabaseClass;

//! Реализация списка классов базы данных PostgreSql
class PgDatabaseClasses
        : public IDatabaseList<PgDatabaseClass, PgDatabaseClass>
{
public:
    //! Конструктор класса
    explicit PgDatabaseClasses(PgDatabaseClass *item);

    //! Деструктор класса
    virtual ~PgDatabaseClasses();

    //! Получение класса базы данных
    PgDatabaseClass *value();
};

}}

#endif // PGDATABASECLASSES_H
