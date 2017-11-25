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
    //! Создание производного класса по имени
    virtual IDatabaseClass *createDerivedClass(
            const QString &name, const QString &base = QString(""));

// Получение доступа к элементам модели по имени
     //! Получение производного класса по имени
    virtual IDatabaseClass *derivedClass(
            const QString &name, const QString &base = QString(""));

// Получение доступа к спискам элементов модели
    //! Cписок всех классов
    virtual IDatabaseClasses *allClassList();

    //! Cписок производных классов модели
    virtual IDatabaseClasses *derivedClassList(const QString &base = QString(""));

private:
    //! Соединение с базой данных
    QSqlDatabase m_db;
};

}}

#endif // PGDATABASEMODEL_H
