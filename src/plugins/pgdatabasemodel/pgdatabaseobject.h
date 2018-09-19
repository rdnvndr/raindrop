#ifndef PGDATABASEOBJECT_H
#define PGDATABASEOBJECT_H

#include <idatabaseobject.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация объекта базы данных PostgreSql
class PgDatabaseObject : public IDatabaseObject
{
public:
    //! Конструктор класса
    explicit PgDatabaseObject();

    //! Деструктор класса
    virtual ~PgDatabaseObject();


    //! Создание элемента базы данных
    void push();

    //! Обновление элемента базы данных
    void pull();

    //! Удаление элемента базы данных
    void remove();
};

}}

#endif // PGDATABASEOBJECT_H
