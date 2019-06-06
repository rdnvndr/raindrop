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
};

}}

#endif // PGDATABASECOMPOSITION_H
