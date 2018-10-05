#ifndef PGDATABASEOBJECT_H
#define PGDATABASEOBJECT_H

#include <idatabaseobject.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация объекта базы данных PostgreSql
class PgDatabaseObject : public IDatabaseObject
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseObject();


    //! Создание элемента базы данных
    void push(IDatabaseThread *databaseThread = nullptr);

    //! Обновление элемента базы данных
    void pull(IDatabaseThread *databaseThread = nullptr);

    //! Удаление элемента базы данных
    void remove(IDatabaseThread *databaseThread = nullptr);
};

}}

#endif // PGDATABASEOBJECT_H
