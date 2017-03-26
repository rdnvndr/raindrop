#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <idatabasemodelmanager.h>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DatabaseModel {

DbModelExample::DbModelExample(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelManager")
{
    PluginManager *pluginManager = PluginManager::instance();
    IDatabaseModelManager *dbModelManager
            = qobject_cast<IDatabaseModelManager *>(
                pluginManager->interfaceObject("IDatabaseModelManager"));

    IDatabaseModel *dbModel = dbModelManager->createInstance();
}

}}

