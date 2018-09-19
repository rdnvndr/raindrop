#ifndef PGDATABASECOMPOSITION_H
#define PGDATABASECOMPOSITION_H

#include <idatabasecomposition.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация состава базы данных PostgreSql
class PgDatabaseComposition : public IDatabaseComposition
{
public:
    //! Конструктор класса
    explicit PgDatabaseComposition();

    //! Деструктор класса
    virtual ~PgDatabaseComposition();


    //! Создаёт элемент базы данных
    void create();

    //! Отправляет изменения элемента базы данных
    void push();

    //! Получает изменения элемента базы данных
    void pull();

    //! Удаляет элемент базы данных
    void remove();
};

}}

#endif // PGDATABASECOMPOSITION_H
