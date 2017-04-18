#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <idatabasemodel.h>
#include <idatabasemodelmanager.h>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DatabaseModel {

DbModelExample::DbModelExample(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelManager")
{
    PluginManager *pluginManager = PluginManager::instance();
    IDatabaseModelManager *dbModelManager
            = pluginManager->interfaceObject<IDatabaseModelManager *>(
                "IDatabaseModelManager");

    IDatabaseModel *dbModel = dbModelManager->createInstance();
}

}}

