#ifndef IDATABASEITEM_H
#define IDATABASEITEM_H

#include <databasemodel/idatabasesession.h>
#include <databasemodel/idatabasepool.h>

#include <QtCore/QUuid>
#include <QtCore/QString>
#include <QtSql/QSqlError>
#include <QtCore/QObject>

//! Элемент базы данных
class IDatabaseItem: public QObject
{
    Q_OBJECT
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
    virtual void create(IDatabaseSession *session = nullptr) = 0;

    //! Отправляет изменения элемента базы данных
    virtual void push(IDatabaseSession *session = nullptr) = 0;

    //! Получает изменения элемента базы данных
    virtual void pull(IDatabaseSession *session = nullptr) = 0;

    //! Удаляет элемент базы данных
    virtual void remove(IDatabaseSession *session = nullptr) = 0;

signals:
    //! Сигнал об окончании выполнения операции в потоке
    void done(QSqlError err);

protected:
    //! Конструктор класса
    explicit IDatabaseItem(IDatabasePool *pool) : QObject() {
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
