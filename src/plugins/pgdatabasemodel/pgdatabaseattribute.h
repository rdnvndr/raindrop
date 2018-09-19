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


    //! Создаёт элемент базы данных
    void create();

    //! Отправляет изменения элемента базы данных
    void push();

    //! Получает изменения элемента базы данных
    void pull();

    //! Удаляет элемент базы данных
    void remove();


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTRIBUTE_H
