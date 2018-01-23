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

    //! Инициализирует модель базы данных
    virtual bool init();

// Создание элемента по имени
    //! Создание класса по имени
    virtual IDatabaseClass *createClass(
            const QString &name, const QString &base = QString(""));

// Получение доступа к элементам модели по имени
     //! Получение класса по имени
    virtual IDatabaseClass *oneClass(
            const QString &name, const QString &base = QString(""));

// Получение доступа к спискам элементов модели
    //! Cписок всех классов модели
    virtual IDatabaseClasses *classList();

    //! Cписок классов модели
    virtual IDatabaseClasses *classList(const QString &base);

private:
    //! Соединение с базой данных
    QSqlDatabase m_db;
};

}}

#endif // PGDATABASEMODEL_H
