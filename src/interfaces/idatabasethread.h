#ifndef IDATABASETHREAD_H
#define IDATABASETHREAD_H

#include <idatabasepool.h>

#include <QUuid>

//! Поток базы данных
class IDatabaseThread
{
public:
    friend class IDatabaseModel;

    //! Деструктор класса
    virtual ~IDatabaseThread() {
        m_pool->release(m_id);
    }

    //! Возвращает идентификатор потока базы данных
    QUuid id() const
    { return m_id; }

// Работа с транзакциями
    //! Начало транзакции
    void transaction() {}

    //! Завершение транзакции
    void commit() {}

    //! Откат транзакции
    void rollback() {}

private:
    //! Конструктор класса
    explicit IDatabaseThread(IDatabasePool *pool) {
        m_id = QUuid();
        m_pool = pool;
        m_pool->acquire(m_id);
    }

    //! Идентификатор потока базы данных
    QUuid m_id;

    //! Пул базы данных
    IDatabasePool *m_pool;
};

#endif // IDATABASETHREAD_H

