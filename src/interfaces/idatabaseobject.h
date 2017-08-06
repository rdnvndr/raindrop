#ifndef IDATABASEOBJECT_H
#define IDATABASEOBJECT_H

#include <idatabaseitem.h>

//! Объект базы данных
class IDatabaseObject: public IDatabaseItem
{
public:
    //! Конструктор класса
    explicit IDatabaseObject() {}

    //! Деструктор класса
    virtual ~IDatabaseObject() {}
};

#endif // IDATABASEOBJECT_H
