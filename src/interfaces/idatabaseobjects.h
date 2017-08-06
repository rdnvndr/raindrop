#ifndef IDATABASEOBJECTS_H
#define IDATABASEOBJECTS_H

#include <idatabaseexpression.h>
#include <idatabaselist.h>

//! Список объектов базы данных
class IDatabaseObjects: public IDatabaseList<IDatabaseClass, IDatabaseObject>
{
public:
    //! Конструктор класса
    explicit IDatabaseObjects(IDatabaseClass *item)
        :IDatabaseList<IDatabaseClass, IDatabaseObject>(item) {}

    //! Деструктор класса
    virtual ~IDatabaseObjects() {}

// Работа с объектами
    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseFilter *filter) = 0;

    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseExpression *expr) = 0;

    //! Получение всех объектов класса
    virtual IDatabaseObjects *all() = 0;

    //! Получение отсортированных объектов
    virtual IDatabaseObjects *orderBy() = 0;

    //! Получение уникальных объектов
    virtual IDatabaseObjects *distinct() = 0;

    //! Сцепление объектов
    virtual IDatabaseObjects *join() = 0;

    //! Объединение объектов
    virtual IDatabaseObjects *unionAll() = 0;

    //! Ограничение объектов
    virtual IDatabaseObjects *limit(int up, int down = 0) = 0;

    //! Ограничение списка возращаемых полей
    virtual IDatabaseObjects *values() = 0;

    //! Получение количества объектов
    virtual int count() = 0;

};

#endif // IDATABASEOBJECT_H
