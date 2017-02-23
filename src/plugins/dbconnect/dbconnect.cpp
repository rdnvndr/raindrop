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
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

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
    delete m_windowConnect;
    delete m_actionDbConnect;
}

void DbConnect::dbConnect()
{
    PluginManager *pluginManager = PluginManager::instance();
    IPlugin *iMainWindow = qobject_cast<IPlugin*>(
                pluginManager->interfaceObject("IMainWindow"));

    m_windowConnect = new DialogConnect(
                qobject_cast<QWidget *>(iMainWindow->instance()));
    m_windowConnect->setWindowTitle(tr("Соединение"));

    settings()->beginGroup("DbConnect");

    m_windowConnect->setDriver(settings()->value("driver").toString());
    m_windowConnect->setDatabaseName(settings()->value("database").toString());
    m_windowConnect->setHostName(settings()->value("hostname").toString());
    m_windowConnect->setPort(settings()->value("port",-1).toInt());

    m_windowConnect->setUserName(settings()->value("username").toString());
    if (m_windowConnect->exec() == QDialog::Accepted) {
        settings()->setValue("driver",   m_windowConnect->driver());
        settings()->setValue("database", m_windowConnect->databaseName());
        settings()->setValue("hostname", m_windowConnect->hostName());
        settings()->setValue("port",     m_windowConnect->port());
        settings()->setValue("username", m_windowConnect->userName());
    }

    settings()->endGroup();
    return;
}

}}
