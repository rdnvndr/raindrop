#ifndef IDATABASEFILTER_H
#define IDATABASEFILTER_H

#include <idatabaseitem.h>
#include <idatabaseexpression.h>
/*
#include <idatabaseclass.h>

class IDatabaseClass;
class IDatabaseFilter;
typedef QHash<QString, IDatabaseFilter *> IDatabaseFilters;
*/

//! Фильтр класса базы данных
class IDatabaseFilter: public IDatabaseItem
{
public:
//    virtual void setExpression(IDatabaseExpression *expr) = 0;
//    virtual IDatabaseFilter orderBy() = 0;
};


#endif
