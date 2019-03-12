#ifndef PGDATABASEMODEL_H
#define PGDATABASEMODEL_H

#include <QtSql/QSqlDatabase>
#include <databasemodel/idatabasemodel.h>

#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация модели базы данных PostgreSql
class PGDATABASEMODELLIB PgDatabaseModel :public QObject,  public IDatabaseModel

{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit PgDatabaseModel(IDatabasePool *pool);

    //! Инициализирует модель базы данных
    virtual QUuid init();

// Получение доступа к элементам модели по имени
     //! Получение класса по имени
    virtual IDatabaseClass *oneClass(const QString &name);

// Получение доступа к спискам элементов модели
    //! Список классов модели
    virtual IDatabaseClasses *classList(const QString &baseClass = QString());

signals:
    //! Сигнал об окончании выполнения операции в потоке
    void done(const QUuid &uuidOper);

    //! Сигнал об ошибке в потоке
    void error(const QUuid &uuidOper, QSqlError err);


private:
    int m_number;
};

}}

#endif // PGDATABASEMODEL_H
