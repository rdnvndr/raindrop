#ifndef PGDATABASEOBJECT_H
#define PGDATABASEOBJECT_H

#include <databasemodel/idatabaseobject.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация объекта базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseObject : public IDatabaseObject
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseObject();


    //! Создание элемента базы данных
    void push(IDatabaseSession *session = nullptr);

    //! Обновление элемента базы данных
    void pull(IDatabaseSession *session = nullptr);

    //! Удаление элемента базы данных
    void remove(IDatabaseSession *session = nullptr);
};

}}

#endif // PGDATABASEOBJECT_H
