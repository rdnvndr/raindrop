#ifndef PGDATABASEATTRIBUTE_H
#define PGDATABASEATTRIBUTE_H

#include <databasemodel/idatabaseattribute.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация атрибута базы данных PostgreSql
class PgDatabaseAttribute : public IDatabaseAttribute
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseAttribute();


    //! Создаёт элемент базы данных
    void create(IDatabaseThread *databaseThread = nullptr);

    //! Отправляет изменения элемента базы данных
    void push(IDatabaseThread *databaseThread = nullptr);

    //! Получает изменения элемента базы данных
    void pull(IDatabaseThread *databaseThread = nullptr);

    //! Удаляет элемент базы данных
    void remove(IDatabaseThread *databaseThread = nullptr);


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTRIBUTE_H
