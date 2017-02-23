#include "dbconnect.h"
#include <plugin/pluginmanager.h>
#include <imainwindow.h>
#include "dialogconnect.h"

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DbConnect {

DbConnect::DbConnect(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionDbConnect = new QAction(QIcon(":connect"), tr("Соединение с  БД"), this);
    connect(actionDbConnect, &QAction::triggered, this, &DbConnect::dbConnect);
    actionDbConnect->setObjectName("actionDbConnect");
    iMainWindow->addAction(tr("Работа с БД"), actionDbConnect);
}

DbConnect::~DbConnect()
{
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    delete actionDbConnect;
}

void DbConnect::dbConnect()
{
    PluginManager *pluginManager = PluginManager::instance();
    IPlugin *iMainWindow = qobject_cast<IPlugin*>(
                pluginManager->interfaceObject("IMainWindow"));

    DialogConnect *windowConnect = new DialogConnect(
                qobject_cast<QWidget *>(iMainWindow->instance()));

    settings()->beginGroup("DbConnect");
    windowConnect->setDriver(settings()->value("driver").toString());
    windowConnect->setDatabaseName(settings()->value("database").toString());
    windowConnect->setHostName(settings()->value("hostname").toString());
    windowConnect->setPort(settings()->value("port",-1).toInt());
    windowConnect->setUserName(settings()->value("username").toString());

    while (true) {
        if (windowConnect->exec() != QDialog::Accepted)
            break;

        if (QSqlDatabase::database().isOpen()) {
            QSqlDatabase::database().close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }

        {
            QSqlDatabase db;

            if (windowConnect->driver() == "MSSQL"){
                db = QSqlDatabase::addDatabase("QODBC");

                db.setDatabaseName(
                            QString("%1;%2;%3").arg("DRIVER={SQL Server}")
                            .arg("DATABASE="+windowConnect->databaseName())
                            .arg("SERVER="+windowConnect->hostName()));
                db.setPort(windowConnect->port());
                db.setUserName(windowConnect->userName());
                db.setPassword(windowConnect->password());
            } else if (windowConnect->driver() == "QOCI") {
                db = QSqlDatabase::addDatabase("QOCI");

                db.setDatabaseName(
                            QString("(DESCRIPTION = "
                                        "(ADDRESS_LIST = "
                                            "(ADDRESS = "
                                                "(PROTOCOL = TCP)"
                                                "(HOST = %1)"
                                                "(PORT = %2)"
                                            ")"
                                        ")"
                                        "(CONNECT_DATA ="
                                            "(SERVER = DEDICATED)"
                                            "(SID =%3)"
                                        ")"
                                    ")")
                            .arg(windowConnect->hostName())
                            .arg(windowConnect->port())
                            .arg(windowConnect->databaseName()));
            } else {
                db = QSqlDatabase::addDatabase(windowConnect->driver());
                db.setDatabaseName(windowConnect->databaseName());
                db.setHostName(windowConnect->hostName());
            }

            db.setPort(windowConnect->port());
            db.setUserName(windowConnect->userName());
            db.setPassword(windowConnect->password());

            if (!db.open()) {
                QSqlError err = db.lastError();
                if (err.type() != QSqlError::NoError)
                    QMessageBox::warning(
                                NULL,
                                tr("Не удается открыть базу данных"),
                                tr("Произошла ошибка при создании соединения:\n")
                                + err.text());
                QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            } else {
                settings()->setValue("driver",   windowConnect->driver());
                settings()->setValue("database", windowConnect->databaseName());
                settings()->setValue("hostname", windowConnect->hostName());
                settings()->setValue("port",     windowConnect->port());
                settings()->setValue("username", windowConnect->userName());
                break;
            }
        }
    }
    settings()->endGroup();
    return;
}

}}
