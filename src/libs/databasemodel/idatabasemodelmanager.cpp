#include "idatabasemodelmanager.h"

#include <plugin/pluginmanager.h>
#include <databasemodel/idatabasemodelbuilder.h>

using namespace RTPTechGroup::Plugin;

IDatabaseModelManager *IDatabaseModelManager::m_instance = nullptr;

IDatabaseModelManager *IDatabaseModelManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new IDatabaseModelManager();
    return m_instance;
}

IDatabaseModelManager::IDatabaseModelManager()
{

}

IDatabaseModelManager::~IDatabaseModelManager()
{

}

IDatabaseModel *IDatabaseModelManager::createModel(QSqlDatabase db)
{
    PluginManager *pluginManager = PluginManager::instance();
    pluginManager->nextLoadPlugins("IDatabaseModelBuilder");
    for (QObject *obj : pluginManager->interfaceObjects("IDatabaseModelBuilder")) {
        IDatabaseModelBuilder *iBuilder = qobject_cast<IDatabaseModelBuilder*>(obj);
        if (iBuilder->implDriverName() == db.driverName()) {
            IDatabasePool  *pool  = new IDatabasePool(db);
            IDatabaseModel *model = iBuilder->createModel(pool);
            return model;
        }
    }
    return nullptr;
}


