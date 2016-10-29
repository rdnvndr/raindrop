#ifndef PGDATABASECOMPOSITION_H
#define PGDATABASECOMPOSITION_H

#include <idatabasecomposition.h>

//! Реализация состава базы данных
class PgDatabaseComposition : public IDatabaseComposition
{
public:
    //! Конструктор класса
    explicit PgDatabaseComposition();

    //! Деструктор класса
    virtual ~PgDatabaseComposition();


    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создание элемента базы данных
    bool push();

    //! Обновление элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();
};

#endif // PGDATABASECOMPOSITION_H
