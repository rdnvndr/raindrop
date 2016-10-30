#ifndef PGDATABASEFILTER_H
#define PGDATABASEFILTER_H

#include <idatabasefilter.h>

class PgDatabaseFilter : public IDatabaseFilter
{
public:
    //! Конструктор класса
    explicit PgDatabaseFilter();

    //! Деструктор класса
    virtual ~PgDatabaseFilter();

    //! Проверка существования элемента базы данных
    bool isExist();

    //! Отправить изменения элемента базы данных
    bool push();

    //! Получить изменения элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();
};

#endif // PGDATABASEFILTER_H
