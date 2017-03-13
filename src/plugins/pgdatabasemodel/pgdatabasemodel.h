#ifndef PGDATABASEMODEL_H
#define PGDATABASEMODEL_H

#include <QtSql/QSqlDatabase>

#include <idatabasemodel.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация модели базы данных PostgreSql
class PgDatabaseModel : public IDatabaseModel
{
public:
    //! Конструктор класса
    explicit PgDatabaseModel(QSqlDatabase db);

// Создание элемента по имени
    //! Создание класса модели по имени
    virtual IDatabaseClass *createModelClass(const QString &name);

// Получение доступа к элементам модели по имени
    //! Получение класса модели по имени
    virtual IDatabaseClass *modelClass(const QString &name);

// Получение доступа к спискам элементов модели
    //! Cписок всех классов
    virtual IDatabaseClasses *classList();

    //! Cписок классов модели
    virtual IDatabaseClasses *modelClassList();

private:
    //! Соединение с базой данных
    QSqlDatabase m_db;
};

}}

#endif // PGDATABASEMODEL_H
