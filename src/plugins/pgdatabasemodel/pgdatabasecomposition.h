#ifndef PGDATABASECOMPOSITION_H
#define PGDATABASECOMPOSITION_H

#include <databasemodel/idatabasecomposition.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация состава базы данных PostgreSql
class PGDATABASEMODELLIB PgDatabaseComposition : public IDatabaseComposition
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseComposition();

    //! Создаёт элемент базы данных
    void create(IDatabaseSession *session = nullptr);

    //! Отправляет изменения элемента базы данных
    void push(IDatabaseSession *session = nullptr);

    //! Получает изменения элемента базы данных
    void pull(IDatabaseSession *session = nullptr);

    //! Удаляет элемент базы данных
    void remove(IDatabaseSession *session = nullptr);
};

}}

#endif // PGDATABASECOMPOSITION_H
