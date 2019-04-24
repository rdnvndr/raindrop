#ifndef PGDATABASEOBJECTS_H
#define PGDATABASEOBJECTS_H

#include <databasemodel/idatabaselist.h>
#include "pgdatabaseclass.h"
#include "pgdatabaseobject.h"
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация списка объектов базы данных PostgreSql
class PGDATABASEMODELLIB PgDatabaseObjects : public IDatabaseList<PgDatabaseClass, PgDatabaseObject>
{
public:
    //! Конструктор класса
    explicit PgDatabaseObjects(PgDatabaseClass *item);

    //! Деструктор класса
    virtual ~PgDatabaseObjects();

    //! Получение объекта базы данных
    PgDatabaseObject *value();
};

}}

#endif // PGDATABASEOBJECTS_H
