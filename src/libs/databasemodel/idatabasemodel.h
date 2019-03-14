#ifndef IDATABASEMODEL_H
#define IDATABASEMODEL_H

#include <databasemodel/idatabaseclass.h>

//! Модель базы данных
class IDatabaseModel
{
public:
    //! Деструктор класса
    virtual ~IDatabaseModel() {
        delete m_pool;
    }

    //! Инициализирует модель базы данных
    virtual QUuid init() = 0;

    //! Создаёт поток базы данных
    virtual IDatabaseSession *createSession() {
        if (m_pool != nullptr)
            return new IDatabaseSession(m_pool);

        return nullptr;
    }

// Получение доступа к элементам модели по имени
    //! Получение класса по имени
    virtual IDatabaseClass *oneClass(const QString &name) = 0;

// Получение доступа к спискам элементов модели
    //! Список классов модели
    virtual IDatabaseClasses *classList(const QString &baseClass = QString()) = 0;

signals:
    //! Сигнал об окончании выполнения операции в потоке
    virtual void done(const QUuid &uuidOper) = 0;

    //! Сигнал об ошибке в потоке
    virtual void error(const QUuid &uuidOper, QSqlError err) = 0;

protected:
    //! Конструктор класса
    explicit IDatabaseModel(IDatabasePool *pool) { m_pool = pool; }

    //! Пул SQL запросов
    IDatabasePool *m_pool;
};

#endif // IDATABASEMODEL_H
