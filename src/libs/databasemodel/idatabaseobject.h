#ifndef IDATABASEOBJECT_H
#define IDATABASEOBJECT_H

#include <databasemodel/idatabaseitem.h>
#include "databasemodelglobal.h"

//! Объект базы данных
class DATABASEMODELLIB IDatabaseObject: public IDatabaseItem
{
public:
    //! Деструктор класса
    virtual ~IDatabaseObject() {}
};

#endif // IDATABASEOBJECT_H
