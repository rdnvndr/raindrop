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
    IDatabaseError create(IDatabaseSession *session = nullptr);

    //! Отправляет изменения элемента базы данных
    IDatabaseError push(IDatabaseSession *session = nullptr);

    //! Получает изменения элемента базы данных
    IDatabaseError pull(IDatabaseSession *session = nullptr);

    //! Переименовывает элемент базы данных
    IDatabaseError rename(const QString &name,
                                     IDatabaseSession *session = nullptr);

    //! Удаляет элемент базы данных
    IDatabaseError remove(IDatabaseSession *session = nullptr);


    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);
};

}}

#endif // PGDATABASEATTRIBUTE_H
