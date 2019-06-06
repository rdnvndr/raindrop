#ifndef IDATABASEERROR_H
#define IDATABASEERROR_H

#include <databasemodel/idatabasesession.h>
#include "databasemodelglobal.h"

#include <QtSql/QSqlError>

class IDatabaseError;

//! Класс функции обработки события
typedef std::function<void(IDatabaseError &)> IHandlerFunction;

//! Структура предоставляет информацию об ошибке
struct IDatabaseErrorStruct
{
    //! Информация об ошибке в базе данных
    QSqlError m_sqlError;

    //! Обработчик события
    IHandlerFunction m_done;

    //! Информация о сессии возникновения ошибки
    IDatabaseSession *m_session;

    //! Обработано ли событие
    bool m_isHandledDone;
};

//! Класс предоставляет информацию об ошибке
class DATABASEMODELLIB IDatabaseError : public std::shared_ptr<IDatabaseErrorStruct>
{
public:
    //! Конструктор класса
    IDatabaseError();

    //! Конструктор класса
    IDatabaseError(IDatabaseSession *session);

    //! Деструктор класса
    ~IDatabaseError();

    //! Возвращает информация о сессии возникновения ошибки
    IDatabaseSession *session() const;

    //! Возвращает true при наличии ошибки, иначе false
    bool isValid() const;

    //! Возвращает информацию об ошибке в базе данных
    QSqlError sqlError() const;

    //! Устанавливает окончание операции
    void finish(const QSqlError &err);

    //! Устанавливает функцию обработки окончания операции
    void setHandlerDone(IHandlerFunction done);

};

#endif // IDATABASEERROR_H
