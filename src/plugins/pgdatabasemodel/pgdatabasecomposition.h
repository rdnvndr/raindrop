#ifndef PGDATABASECOMPOSITION_H
#define PGDATABASECOMPOSITION_H

#include <databasemodel/idatabasecomposition.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация состава базы данных PostgreSql
class PgDatabaseComposition : public IDatabaseComposition
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseComposition();

    //! Создаёт элемент базы данных
    void create(IDatabaseThread *databaseThread = nullptr);

    //! Отправляет изменения элемента базы данных
    void push(IDatabaseThread *databaseThread = nullptr);

    //! Получает изменения элемента базы данных
    void pull(IDatabaseThread *databaseThread = nullptr);

    //! Удаляет элемент базы данных
    void remove(IDatabaseThread *databaseThread = nullptr);
};

}}

#endif // PGDATABASECOMPOSITION_H
