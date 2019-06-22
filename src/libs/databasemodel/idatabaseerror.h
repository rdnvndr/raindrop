#ifndef IDATABASEERROR_H
#define IDATABASEERROR_H

#include <databasemodel/idatabasesession.h>
#include <memory>
#include "databasemodelglobal.h"

#include <QtSql/QSqlError>

class IDatabaseError;

//! Класс функции обработки события
typedef std::function<void(IDatabaseError &)> IHandlerDone;

//! Структура предоставляет информацию об ошибке
struct IDatabaseErrorStruct
{
    QSqlError m_sqlError;        //! Информация об ошибке в базе данных
    IHandlerDone m_done;         //! Обработчик события
    IDatabaseSession *m_session; //! Информация о сессии возникновения ошибки
    bool m_isHandled;            //! Обработано ли событие
};

//! Класс предоставляет информацию об ошибке
class DATABASEMODELLIB IDatabaseError : public std::shared_ptr<IDatabaseErrorStruct>
{
public:
    //! Конструктор класса
    IDatabaseError();

    //! Конструктор класса
    IDatabaseError(IDatabaseSession *session);

    //! Возвращает информация о сессии возникновения ошибки
    IDatabaseSession *session() const;

    //! Возвращает true при наличии ошибки, иначе false
    bool isValid() const;

    //! Возвращает информацию об ошибке в базе данных
    QSqlError sqlError() const;

    //! Устанавливает окончание операции
    void finish(const QSqlError &err);

    //! Устанавливает функцию обработки окончания операции
    void setHandlerDone(IHandlerDone done);

    //! Возвращает функцию обработки окончания операции
    IHandlerDone handlerDone();
};

#endif // IDATABASEERROR_H
