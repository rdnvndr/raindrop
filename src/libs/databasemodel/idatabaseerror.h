#ifndef IDATABASEERROR_H
#define IDATABASEERROR_H

#include <databasemodel/idatabasesession.h>
#include "databasemodelglobal.h"

#include <QtSql/QSqlError>

//! Класс предоставляет информацию об ошибке
class DATABASEMODELLIB IDatabaseError
{
public:
    //! Конструктор класса
    IDatabaseError(const QSqlError &err, IDatabaseSession *s = nullptr);

    //! Возвращает информация о сессии возникновения ошибки
    IDatabaseSession *session() const;

    //! Возвращает true при наличии ошибки, иначе false
    bool isValid() const;

private:
    //! Информация об ошибке в базе данных
    QSqlError m_sqlError;

    //! Информация о сессии возникновения ошибки
    IDatabaseSession *m_session;
};

#endif // IDATABASEERROR_H
