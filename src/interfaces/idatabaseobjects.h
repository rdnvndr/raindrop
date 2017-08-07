#ifndef IDATABASEOBJECTS_H
#define IDATABASEOBJECTS_H

#include <idatabaselist.h>
#include <idatabaseclass.h>
#include <idatabaseobject.h>
#include <idatabasemanipulation.h>

//! Список объектов базы данных
class IDatabaseObjects: public IDatabaseList<IDatabaseClass, IDatabaseObject>,
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
