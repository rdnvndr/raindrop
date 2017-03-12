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

private:
    //! Соединение с базой данных
    QSqlDatabase m_db;
};

}}

#endif // PGDATABASEMODEL_H
