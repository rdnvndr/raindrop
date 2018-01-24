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


    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создаёт элемент базы данных
    bool create();

    //! Отправляет изменения элемента базы данных
    bool push();

    //! Получает изменения элемента базы данных
    bool pull();

    //! Удаляет элемент базы данных
    bool remove();
};

}}

#endif // PGDATABASECOMPOSITION_H
