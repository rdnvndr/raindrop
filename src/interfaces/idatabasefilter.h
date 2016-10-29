#ifndef IDATABASEFILTER_H
#define IDATABASEFILTER_H

#include <idatabaseitem.h>
#include <idatabaseexpression.h>

//! Фильтр класса базы данных
class IDatabaseFilter: public IDatabaseItem
{
public:
    //! Конструктор класса
    explicit IDatabaseFilter() {};

    //! Деструктор класса
    virtual ~IDatabaseFilter() {};

//    virtual void setExpression(IDatabaseExpression *expr) = 0;
//    virtual IDatabaseFilter orderBy() = 0;

};


#endif
