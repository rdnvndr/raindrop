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


    //! Отправить изменения элемента базы данных
    virtual void push();

    //! Получить изменения элемента базы данных
    virtual void pull();

    //! Удаление элемента базы данных
    virtual void remove();

// Получение доступа к элементам класса по имени
    //! Получение производного класса по имени
    IDatabaseClass *derivedClass(const QString &name) ;

    //! Получение атрибута по имени
    IDatabaseAttribute *attr(const QString &name);

    //! Получение фильтра по имени
    IDatabaseFilter *filter(const QString &name);

    //! Получение состава по имени
    IDatabaseComposition *comp(const QString &name);

// Получение доступа к спискам элементов класса по имени
    //! Список производных классов
    IDatabaseClasses *derivedClassList();

    //! Список атрибутов в классе
    IDatabaseAttrs *attrList();

    //! Список фильтров в классе
    IDatabaseFilters *filterList();

    //! Список атрибутов в классе
    IDatabaseComps *compList();

// Работа с объектами
     //! Получение отфильтрованных объектов класса
    IDatabaseObjects *filtering(IDatabaseFilter *filter);

    //! Получение отфильтрованных объектов класса
    IDatabaseObjects *filtering(IDatabaseExpression *expr);

    //! Получение всех объектов класса
    IDatabaseObjects *all();
};

}}

#endif // PGDATABASECLASS_H
