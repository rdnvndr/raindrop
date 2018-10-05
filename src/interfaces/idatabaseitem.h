#ifndef IDATABASEITEM_H
#define IDATABASEITEM_H

#include <idatabasethread.h>
#include <idatabasepool.h>

#include <QUuid>
#include <QString>
#include <QtSql/QSqlDatabase>
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
    virtual void setId(QUuid id) { m_id = id; }

    //! Возвращает имя элемента базы данных
    virtual QString name() {return m_name; }

    //! Устанавливает имя элемента базы данных
    virtual void setName(const QString &name) { m_name = name; }

    //! Возвращает псевдоним элемента базы данных
    virtual QString alias() { return m_alias; }

    //! Устанавливает псевдоним элемента базы данных
    virtual void setAlias(const QString &alias) { m_alias = alias; }

    //! Проверяет проведена ли синхронизацию с базой данных
    virtual bool isSync() { return m_sync; }

    //! Создаёт элемент базы данных
    virtual void create() = 0;

    //! Отправляет изменения элемента базы данных
    virtual void push() = 0;

    //! Получает изменения элемента базы данных
    virtual void pull() = 0;

    //! Удаляет элемент базы данных
    virtual void remove() = 0;

    //! Устанавливает поток базы данных
    virtual void setDatabaseThread(IDatabaseThread *databaseThread)
    { m_threadUuid = databaseThread->id(); }

signals:
    //! Сигнал об окончании выполнения операции в потоке
    void done();

    //! Сигнал об ошибке в потоке
    void error(QSqlError err);

protected:
    //! Конструктор класса
    explicit IDatabaseItem(IDatabasePool *pool) {
        m_threadUuid = QUuid();
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

    //! Поток базы данных
    QUuid m_threadUuid;

    //! Получены ли данные хотя бы один раз
    bool m_sync;
};

#endif // IDATABASEITEM_H
