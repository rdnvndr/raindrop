#ifndef PGDATABASEATTRIBUTE_H
#define PGDATABASEATTRIBUTE_H

#include <databasemodel/idatabaseattribute.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация атрибута базы данных PostgreSql
class PGDATABASEMODELLIB PgDatabaseAttribute : public IDatabaseAttribute
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseAttribute();


    //! Создаёт элемент базы данных
    void create(IDatabaseSession *session = nullptr);

    //! Отправляет изменения элемента базы данных
    void push(IDatabaseSession *session = nullptr);

    //! Получает изменения элемента базы данных
    void pull(IDatabaseSession *session = nullptr);

    //! Удаляет элемент базы данных
    void remove(IDatabaseSession *session = nullptr);


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTRIBUTE_H
