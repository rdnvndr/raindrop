#ifndef IDATABASEOBJECTS_H
#define IDATABASEOBJECTS_H

#include <databasemodel/idatabaselist.h>
#include <databasemodel/idatabaseclass.h>
#include <databasemodel/idatabaseobject.h>
#include <databasemodel/idatabasemanipulation.h>
#include "databasemodelglobal.h"

//! Список объектов базы данных
class DATABASEMODELLIB IDatabaseObjects: public IDatabaseList<IDatabaseClass, IDatabaseObject>,
                        public IDatabaseManipulation
{
public:
    //! Конструктор класса
    explicit IDatabaseObjects(IDatabaseClass *item)
        :IDatabaseList<IDatabaseClass, IDatabaseObject>(item),
         IDatabaseManipulation(item) {}

    //! Деструктор класса
    virtual ~IDatabaseObjects() {}

};

#endif // IDATABASEOBJECT_H
