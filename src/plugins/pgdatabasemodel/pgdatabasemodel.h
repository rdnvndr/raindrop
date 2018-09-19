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
    explicit PgDatabaseModel(IDatabasePool *pool);

    //! Инициализирует модель базы данных
    virtual bool init();

// Создание элемента по имени
    //! Создание класса по имени
    virtual IDatabaseClass *createClass(
            const QString &name, const QString &base = QString(""));

// Получение доступа к элементам модели по имени
     //! Получение класса по имени
    virtual IDatabaseClass *oneClass(const QString &name);

// Получение доступа к спискам элементов модели
    //! Список всех классов модели
    virtual IDatabaseClasses *classList();

    //! Список производных классов модели
    virtual IDatabaseClasses *derivedClassList(const QString &base);
};

}}

#endif // PGDATABASEMODEL_H
