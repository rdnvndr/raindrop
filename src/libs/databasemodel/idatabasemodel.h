#ifndef IDATABASEMODEL_H
#define IDATABASEMODEL_H

#include <databasemodel/idatabaseclass.h>
#include "databasemodelglobal.h"

//! Модель базы данных
class DATABASEMODELLIB IDatabaseModel : public QObject
{
    Q_OBJECT
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
    void done(const QUuid &uuidOper, const QSqlError &err);

protected:
    //! Конструктор класса
    explicit IDatabaseModel(IDatabasePool *pool) : QObject () { m_pool = pool; }

    //! Пул SQL запросов
    IDatabasePool *m_pool;
};

#endif // IDATABASEMODEL_H
