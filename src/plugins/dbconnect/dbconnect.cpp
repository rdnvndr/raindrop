#include "dbconnect.h"

#include "threadconnect.h"

#include <plugin/pluginmanager.h>
#include <imainwindow.h>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DbConnect {

DbConnect::DbConnect(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");

    m_actionDbConnect = new QAction(QIcon(":connect"), tr("Соединение с  БД"), this);
    connect(m_actionDbConnect, &QAction::triggered, this, &DbConnect::dbConnect);
    m_actionDbConnect->setObjectName("actionDbConnect");
    iMainWindow->addAction(tr("Работа с БД"), m_actionDbConnect);
}

DbConnect::~DbConnect()
{
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
    delete m_actionDbConnect;
}

void DbConnect::dbConnect()
{
    PluginManager *pluginManager = PluginManager::instance();    
    IPlugin *iMainWindow
            = pluginManager->interfaceObject<IPlugin*>("IMainWindow");

    DialogConnect *windowConnect = new DialogConnect(
                iMainWindow->instance<QWidget *>());
    windowConnect->setWindowTitle(tr("Соединение"));

    settings()->beginGroup("DbConnect");

    windowConnect->setDriverName(settings()->value("driver").toString());
    windowConnect->setDatabaseName(settings()->value("database").toString());
    windowConnect->setHostName(settings()->value("hostname").toString());
    windowConnect->setPort(settings()->value("port",-1).toInt());
    windowConnect->setUserName(settings()->value("username").toString());

    if (windowConnect->exec() == QDialog::Accepted) {
        settings()->setValue("driver",   windowConnect->driverName());
        settings()->setValue("database", windowConnect->databaseName());
        settings()->setValue("hostname", windowConnect->hostName());
        settings()->setValue("port",     windowConnect->port());
        settings()->setValue("username", windowConnect->userName());
    }
    delete windowConnect;

    settings()->endGroup();
    return;
}

}}
