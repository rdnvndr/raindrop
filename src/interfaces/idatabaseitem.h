#ifndef IDATABASEITEM_H
#define IDATABASEITEM_H

#include <idatabasethread.h>
#include <idatabasepool.h>

#include <QUuid>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtCore/QObject>

//! Элемент базы данных
class IDatabaseItem
{

public:
    //! Деструктор класса
    virtual ~IDatabaseItem() {}

    //! Возвращает идентификатор элемента базы данных
    virtual QUuid id() { return m_id; }

    //! Устанавливает идентификатор элемента базы данных
    virtual void setId(QUuid id) {
        m_sync = false;
        m_id = id;
    }

    //! Возвращает имя элемента базы данных
    virtual QString name() {return m_name; }

    //! Устанавливает имя элемента базы данных
    virtual void setName(const QString &name) {
        m_sync = false;
        m_name = name;
    }

    //! Возвращает псевдоним элемента базы данных
    virtual QString alias() { return m_alias; }

    //! Устанавливает псевдоним элемента базы данных
    virtual void setAlias(const QString &alias) {
        m_sync = false;
        m_alias = alias;
    }

    //! Проверяет проведена ли синхронизацию с базой данных
    virtual bool isSync() { return m_sync; }

    //! Создаёт элемент базы данных
    virtual void create(IDatabaseThread *databaseThread = nullptr) = 0;

    //! Отправляет изменения элемента базы данных
    virtual void push(IDatabaseThread *databaseThread = nullptr) = 0;

    //! Получает изменения элемента базы данных
    virtual void pull(IDatabaseThread *databaseThread = nullptr) = 0;

    //! Удаляет элемент базы данных
    virtual void remove(IDatabaseThread *databaseThread = nullptr) = 0;

signals:
    //! Сигнал об окончании выполнения операции в потоке
    virtual void done() = 0;

    //! Сигнал об ошибке в потоке
    virtual void error(QSqlError err) = 0;

protected:
    //! Конструктор класса
    explicit IDatabaseItem(IDatabasePool *pool) {
        m_sync = false;
        m_pool = pool;
    }

    //! Пул SQL запросов
    IDatabasePool *m_pool;

    //! Идентификатор элемента базы данных
    QUuid m_id;

    //! Наименование элемента базы данных
    QString m_name;

    //! Псевдоним элемента базы данных
    QString m_alias;

    //! Получены ли данные хотя бы один раз
    bool m_sync;
};

#endif // IDATABASEITEM_H
