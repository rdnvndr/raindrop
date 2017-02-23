#ifndef DIALOGCONNECT_H
#define	DIALOGCONNECT_H

#include "ui_dialogconnect.h"
#include "threadconnect.h"

#include <QtSql>

namespace RTPTechGroup {
namespace DbConnect {

//! Диалоговое окно подключения к БД
class DialogConnect: public QDialog, private Ui::DialogConnect{

    Q_OBJECT
public:
    //! Конструктор диалогового окна подключения к БД
    explicit DialogConnect(QWidget *pwgt = 0);

    //! Деструктор диалогового окна подключения к БД
    virtual ~DialogConnect();

    //! Вызов успешного завершения
    void accept() override;

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

protected:
    //! Слот обработки события закрытия окна
    void closeEvent(QCloseEvent *event) override;

private:
    void setLockDialog(bool locked);

private slots:
    //! Начало соединения
    void startConnect();

    //! Окончание открытия соединения
    void finishConnect(QString result);

public slots:
    //! Слот для отображения дополнительных параметров подключения к БД
    void onClickButtonProp();

private:
     ThreadConnect *threadConnect;
};

}}

#endif
