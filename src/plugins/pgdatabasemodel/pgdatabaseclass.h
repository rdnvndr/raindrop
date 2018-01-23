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

// Получение доступа к элементам класса по имени
    //! Получение производного класса по имени
    IDatabaseClass *oneClass(const QString &name) ;

    //! Получение атрибута по имени
    IDatabaseAttribute *attr(const QString &name);

    //! Получение фильтра по имени
    IDatabaseFilter *filter(const QString &name);

    //! Получение состава по имени
    IDatabaseComposition *comp(const QString &name);

// Получение доступа к спискам элементов класса по имени
    //! Cписок производных классов
    IDatabaseClasses *classList();

    //! Cписок атрибутов в классе
    IDatabaseAttrs *attrList();

    //! Cписок фильтров в классе
    IDatabaseFilters *filterList();

    //! Cписок атрибутов в классе
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
