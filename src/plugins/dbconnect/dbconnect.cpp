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
    PluginManager* pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionDbConnect = new QAction(QIcon(":connect"), tr("Соединение с  БД"), this);
    connect(actionDbConnect, SIGNAL(triggered()), this, SLOT(dbConnect()));
    actionDbConnect->setObjectName("actionDbConnect");
    iMainWindow->addAction(tr("Работа с БД"),actionDbConnect);
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
    PluginManager* pluginManager = PluginManager::instance();
    IPlugin* iMainWindow = qobject_cast<IPlugin*>(
                pluginManager->interfaceObject("IMainWindow"));

    DialogConnect* windowConnect = new DialogConnect(
                qobject_cast<QWidget *>(iMainWindow->instance()));
    settings()->beginGroup("DbConnect");
    windowConnect->comboDriver->setCurrentIndex(
                windowConnect->comboDriver->findText(settings()->value("driver").toString()));
    windowConnect->editDatabase->setText(settings()->value("database").toString());
    windowConnect->editHostname->setText(settings()->value("hostname").toString());
    windowConnect->portSpinBox->setValue(settings()->value("port",-1).toInt());
    windowConnect->editUsername->setText(settings()->value("username").toString());
    while (true) {
        if (windowConnect->exec() == QDialog::Accepted) {
            if (QSqlDatabase::database().isOpen()) {
                QSqlDatabase::database().close();
                QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
            }
            {
                QSqlDatabase db;


                if (windowConnect->comboDriver->currentText()=="MSSQL"){
                    db = QSqlDatabase::addDatabase("QODBC");
                    db.setDatabaseName(QString("%1;%2;%3").arg("DRIVER={SQL Server}").
                                       arg("DATABASE="+windowConnect->editDatabase->text()).
                                       arg("SERVER="+windowConnect->editHostname->text())
                                       );
                    db.setPort(windowConnect->portSpinBox->value());
                    db.setUserName(windowConnect->editUsername->text());
                    db.setPassword(windowConnect->editPassword->text());
                } else if (windowConnect->comboDriver->currentText()=="QOCI"){
                    db = QSqlDatabase::addDatabase("QOCI");

                    db.setDatabaseName(QString("(DESCRIPTION = (ADDRESS_LIST = (ADDRESS = (PROTOCOL = TCP)(HOST = %1)(PORT = %2)))(CONNECT_DATA =(SERVER = DEDICATED)(SID =%3)))").
                                       arg(windowConnect->editHostname->text()).
                                       arg(windowConnect->portSpinBox->value()).
                                       arg(windowConnect->editDatabase->text())
                                       );
                }
                else {
                    db = QSqlDatabase::addDatabase(windowConnect->comboDriver->currentText());
                    db.setDatabaseName(windowConnect->editDatabase->text());
                    db.setHostName(windowConnect->editHostname->text());
                }

                db.setPort(windowConnect->portSpinBox->value());
                db.setUserName(windowConnect->editUsername->text());
                db.setPassword(windowConnect->editPassword->text());

                if (!db.open()) {
                    QSqlError err = db.lastError();
                    if (err.type() != QSqlError::NoError)
                        QMessageBox::warning(NULL, tr("Не удается открыть базу данных"), tr("Произошла ошибка при создании соединения: ") + err.text());
                }
                else {
                    // OK
                    settings()->setValue("driver", windowConnect->comboDriver->currentText());
                    settings()->setValue("database", windowConnect->editDatabase->text());
                    settings()->setValue("hostname", windowConnect->editHostname->text());
                    settings()->setValue("port", windowConnect->portSpinBox->value());
                    settings()->setValue("username", windowConnect->editUsername->text());
                    break;
                }
            }
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }
        else {
            // NOT OK
            settings()->endGroup();
            return;
        }
    }
    settings()->endGroup();
    return;
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dbconnect, DbConnect)
#endif



