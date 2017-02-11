#ifndef IDATABASEITEM_H
#define IDATABASEITEM_H

#include <QUuid>
#include <QString>
#include <QtSql/QSqlDatabase>

//! Элемент базы данных
class IDatabaseItem
{

public:
    //! Возращает идентификатор элемента базы данных
    virtual QUuid id() { return m_id; }

    //! Устанавливает идентификатор элемента базы данных
    virtual void setId(QUuid id) { m_id = id; }

    //! Возращает имя элемента базы данных
    virtual QString name() {return m_name; }

    //! Устанавливает имя элемента базы данных
    virtual void setName(const QString &name) { m_name = name; }

    //! Возращает псевдоним элемента базы данных
    virtual QString alias() { return m_alias; }

    //! Устанавливает псевдоним элемента базы данных
    virtual void setAlias(const QString &alias) { m_alias = alias; }

    //! Проверка существования элемента базы данных
    virtual bool isExist() = 0;

    //! Отправить изменения элемента базы данных
    virtual bool push() = 0;

    //! Получить изменения элемента базы данных
    virtual bool pull() = 0;

    //! Удаление элемента базы данных
    virtual bool remove() = 0;

    //! Получения соединения с базой данных
    virtual QSqlDatabase database() const { return m_database; }

    //! Установка соединения с базой данных
    virtual void setDatabase(const QSqlDatabase &database)
    { m_database = database; }

private:
    //! Соединение с базой данных
    QSqlDatabase m_database;

    //! Индетификатор элемента базы данных
    QUuid m_id;

    //! Наименование элемента базы данных
    QString m_name;

    //! Псевдоним элемента базы данных
    QString m_alias;
};

#endif // IDATABASEITEM_H
