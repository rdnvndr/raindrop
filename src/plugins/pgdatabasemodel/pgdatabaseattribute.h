#ifndef PGDATABASEATTRIBUTE_H
#define PGDATABASEATTRIBUTE_H

#include <idatabaseattribute.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация атрибута базы данных PostgreSql
class PgDatabaseAttribute : public IDatabaseAttribute
{
public:
    //! Конструктор класса
    explicit PgDatabaseAttribute();

    //! Деструктор класса
    virtual ~PgDatabaseAttribute();


    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создаёт элемент базы данных
    bool create();

    //! Отправляет изменения элемента базы данных
    bool push();

    //! Получает изменения элемента базы данных
    bool pull();

    //! Удаляет элемент базы данных
    bool remove();


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTRIBUTE_H
