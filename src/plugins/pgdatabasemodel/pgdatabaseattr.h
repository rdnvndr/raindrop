#ifndef PGDATABASEATTR_H
#define PGDATABASEATTR_H

#include <idatabaseattribute.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация атрибута базы данных PostgreSql
class PgDatabaseAttr : public IDatabaseAttribute
{
public:
    //! Конструктор класса
    explicit PgDatabaseAttr();

    //! Деструктор класса
    virtual ~PgDatabaseAttr();


    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создание элемента базы данных
    bool push();

    //! Обновление элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTR_H
