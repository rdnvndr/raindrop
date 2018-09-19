#ifndef IDATABASEPOOL_H
#define IDATABASEPOOL_H

#include <idatabasethread.h>
#include <sqlextension/threadquery.h>

#include <QUuid>


//! Модель пул базы данных
class IDatabasePool
{

public:
    //! Конструктор класса
    explicit IDatabasePool() {}

    //! Деструктор класса
    virtual ~IDatabasePool() {}

    //! Резервирует sql запрос потока
    virtual void acquire(QUuid threadUuid) = 0;

    //! Получение sql запроса потока
    virtual RTPTechGroup::SqlExtension::ThreadQuery *threadQuery(QUuid threadUuid) = 0;

    //! Возвращает sql запрос потока
    virtual void release(QUuid threadUuid) = 0;
};



#endif // IDATABASEPOOL_H
