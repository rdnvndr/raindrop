#ifndef IDATABASEOBJECT_H
#define IDATABASEOBJECT_H

#include <idatabaseitem.h>

//! Объект базы данных
class IDatabaseObject: public IDatabaseItem
{
public:
    //! Деструктор класса
    virtual ~IDatabaseObject() {}
};

#endif // IDATABASEOBJECT_H
