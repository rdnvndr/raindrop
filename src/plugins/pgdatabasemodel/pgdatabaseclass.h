#ifndef PGDATABASECLASS_H
#define PGDATABASECLASS_H

#include <idatabaseclass.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация класса базы данных PostgreSql
class PgDatabaseClass : public IDatabaseClass
{
public:
    //! Конструктор класса
    explicit PgDatabaseClass();

    //! Деструктор класса
    virtual ~PgDatabaseClass();


    //! Проверка существования элемента базы данных
    virtual bool isExist();

    //! Отправить изменения элемента базы данных
    virtual bool push();

    //! Получить изменения элемента базы данных
    virtual bool pull();

    //! Удаление элемента базы данных
    virtual bool remove();


    //! Cписок атрибутов в классе
    IDatabaseAttrs attrList();

    //! Cписок фильтров в классе
    IDatabaseFilters filterList();

//! Работа с объектами
     //! Получение отфильтрованных объектов класса
    void filtering(IDatabaseFilter *filter);

    //! Получение отфильтрованных объектов класса
    void filtering(IDatabaseExpression *expr);

    //! Получение всех объектов класса
    void all();
};

}}

#endif // PGDATABASECLASS_H
