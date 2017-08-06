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


    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создание элемента базы данных
    bool push();

    //! Обновление элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();
};

}}

#endif // PGDATABASEOBJECT_H
