#ifndef PGDATABASEOBJECT_H
#define PGDATABASEOBJECT_H

#include <databasemodel/idatabaseobject.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация объекта базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseObject : public IDatabaseObject
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseObject();

// Работа с элементами базы данных
    //! Создаёт элемент базы данных
    IDatabaseError create(IDatabaseSession *session = nullptr);

    //! Отправить изменения элемента базы данных
    IDatabaseError push(IDatabaseSession *session = nullptr);

    //! Получить изменения элемента базы данных
    IDatabaseError pull(IDatabaseSession *session = nullptr);

    //! Переименовывает элемент базы данных
    IDatabaseError rename(const QString &name,
                                     IDatabaseSession *session = nullptr);

    //! Удаление элемента базы данных
    IDatabaseError remove(IDatabaseSession *session = nullptr);
};

}}

#endif // PGDATABASEOBJECT_H
