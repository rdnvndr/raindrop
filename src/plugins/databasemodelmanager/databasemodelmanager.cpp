#include "databasemodelmanager.h"

#include <plugin/pluginmanager.h>
#include "databasepool.h"
using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DatabaseModel {

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
    for (QObject *obj : pluginManager->interfaceObjects("IDatabaseModelBuilder")) {
        IDatabaseModelBuilder *iBuilder = qobject_cast<IDatabaseModelBuilder*>(obj);
        if (iBuilder->implDriverName() == db.driverName()) {
            IDatabasePool  *pool  = new DatabasePool(db);
            IDatabaseModel *model = iBuilder->createDatabaseModel(pool);
            return model;
        }
    }

    return nullptr;
}

}}
