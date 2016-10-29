#ifndef PGDATABASECOMPOSITION_H
#define PGDATABASECOMPOSITION_H

#include <idatabasecomposition.h>

//! Реализация состава базы данных
class PgDatabaseComposition : public IDatabaseComposition
{
public:
    //! Конструктор класса
    explicit PgDatabaseComposition();

    //! Деструктор класса
    virtual ~PgDatabaseComposition();

    //! Возращает идентификатор элемента базы данных
    QUuid id();

    //! Устанавливает идентификатор элемента базы данных
    void setId(QUuid id);

    //! Возращает имя элемента базы данных
    QString name();

    //! Устанавливает имя элемента базы данных
    void setName(const QString &name);

    //! Возращает псевдоним элемента базы данных
    QString alias();

    //! Устанавливает псевдоним элемента базы данных
    void setAlias(const QString &alias);

    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создание элемента базы данных
    bool push();

    //! Обновление элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();
};

#endif // PGDATABASECOMPOSITION_H
