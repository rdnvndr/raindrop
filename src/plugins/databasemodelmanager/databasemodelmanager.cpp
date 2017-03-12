#include "databasemodelmanager.h"

#include <plugin/pluginmanager.h>
using namespace RTPTechGroup::Plugin;

DatabaseModelManager::DatabaseModelManager(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelBuilder")
{

}

DatabaseModelManager::~DatabaseModelManager()
{

}

IDatabaseModel *DatabaseModelManager::createInstance(QSqlDatabase db)
{
    PluginManager *pluginManager = PluginManager::instance();
    foreach (QObject *obj, pluginManager->interfaceObjects("IDatabaseModelBuilder")) {
        IDatabaseModelBuilder *iBuilder = qobject_cast<IDatabaseModelBuilder*>(obj);
        if (iBuilder->implDriverName() == db.driverName())
            return iBuilder->createDatabaseModel(db);
    }

    return NULL;
}

