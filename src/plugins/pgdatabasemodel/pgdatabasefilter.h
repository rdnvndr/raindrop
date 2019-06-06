#ifndef PGDATABASEFILTER_H
#define PGDATABASEFILTER_H

#include <databasemodel/idatabasefilter.h>
#include "pgdatabasemodelglobal.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация фильтра базы данных PostgreSql
class PGDATABASEMODELLIB  PgDatabaseFilter : public IDatabaseFilter
{
public:
    //! Деструктор класса
    virtual ~PgDatabaseFilter();

    //! Отправить изменения элемента базы данных
    IDatabaseError push(IDatabaseSession *session = nullptr);

    //! Получить изменения элемента базы данных
    IDatabaseError pull(IDatabaseSession *session = nullptr);

    //! Переименовывает элемент базы данных
    IDatabaseError rename(const QString &name,
                                     IDatabaseSession *session = nullptr);

    //! Удаление элемента базы данных
    IDatabaseError remove(IDatabaseSession *session = nullptr);
};

}}

#endif // PGDATABASEFILTER_H
