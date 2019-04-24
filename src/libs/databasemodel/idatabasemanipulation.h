#ifndef IDATABASEMANIPULATION_H
#define IDATABASEMANIPULATION_H

#include <databasemodel/idatabaseclass.h>
#include <databasemodel/idatabaseattribute.h>
#include <databasemodel/idatabaseexpression.h>
#include <databasemodel/idatabasefilter.h>
#include "databasemodelglobal.h"

class IDatabaseObjects;

//! Работа с объектами базы данных
class DATABASEMODELLIB IDatabaseManipulation
{
public:
    //! Перечисление типов сцепления
    enum JoinType {Inner, LeftOuter, RightOuter, FullOuter, Cross};

    //! Конструктор класса
    explicit IDatabaseManipulation(IDatabaseClass *item) { Q_UNUSED(item) }

    //! Деструктор класса
    virtual ~IDatabaseManipulation() {}

// Работа с объектами
    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseFilter *filter) = 0;

    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseExpression *expr) = 0;


    //! Получение отсортированных объектов по возрастанию
    virtual IDatabaseObjects *orderByAsc(IDatabaseAttribute *attr, ...) = 0;

    //! Получение отсортированных объектов по убыванию
    virtual IDatabaseObjects *orderByDesc(IDatabaseAttribute *attr, ...) = 0;

    //! Получение уникальных объектов
    virtual IDatabaseObjects *distinct(IDatabaseAttribute *attr, ...) = 0;

    //! Ограничение объектов
    virtual IDatabaseObjects *limit(int up, int down = 0) = 0;

    //! Ограничение списка возвращаемых полей
    virtual IDatabaseObjects *values(IDatabaseAttribute *attr = nullptr, ...) = 0;


    //! Сцепление объектов
    virtual IDatabaseObjects *join(IDatabaseFilter *filter,
                                   JoinType joinType = Inner) = 0;

    //! Сцепление объектов
    virtual IDatabaseObjects *join(IDatabaseExpression *expr,
                                   JoinType joinType = Inner) = 0;

    //! Объединение объектов
    virtual IDatabaseObjects *unionAll(IDatabaseObjects *objs) = 0;


    //! Получение количества объектов
    virtual int count(IDatabaseAttribute *attr = nullptr,
                      bool distinct = false) = 0;

    //! Получение среднего значения атрибута
    virtual int avg(IDatabaseAttribute *attr) = 0;

    //! Получение минимального значения атрибута
    virtual int min(IDatabaseAttribute *attr) = 0;

    //! Получение максимального значения атрибута
    virtual int max(IDatabaseAttribute *attr) = 0;

    //! Получение суммарного значения атрибута
    virtual int sum(IDatabaseAttribute *attr) = 0;

};

#endif // IDATABASEMANIPULATION_H
