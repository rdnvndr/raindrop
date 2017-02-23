#ifndef DIALOGCONNECT_H
#define	DIALOGCONNECT_H
#include "ui_dialogconnect.h"
#include <QtSql>

namespace RTPTechGroup {
namespace DbConnect {

//! Диалоговое окно подключения к БД
class DialogConnect: public QDialog, private Ui::DialogConnect{

    Q_OBJECT
public:
    //! Коструктор диалогового окна подключения к БД
    explicit DialogConnect(QWidget *pwgt = 0);

    //! Установка драйвера БД
    void setDriver(const QString &name);

    //! Установка имени БД
    void setDatabaseName(const QString &name);

    //! Установка имени хоста
    void setHostName(const QString &host);

    //! Установка имени пользователя
    void setUserName(const QString &name);

    //! Установка пароля
    void setPassword(const QString &password);

    //! Установка порта
    void setPort(int port);

    //! Получение драйвера БД
    QString driver() const;

    //! Получение имени БД
    QString databaseName() const;

    //! Получение имени хоста
    QString hostName() const;

    //! Получение имени пользователя
    QString userName() const;

    //! Получение пароля
    QString password() const;

    //! Получение порта
    int port() const;

public slots:
    //! Слот для отображения дополнительных параметров подключения к БД
    void onClickButtonProp();
};

}}

#endif
