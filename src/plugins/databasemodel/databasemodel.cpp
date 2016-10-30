#include "databasemodel.h"

#include <plugin/pluginmanager.h>
using namespace RTPTechGroup::Plugin;

DatabaseModel::DatabaseModel(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelBuilder")
{

}

DatabaseModel::~DatabaseModel()
{

}

IDatabaseModelBuilder *DatabaseModel::modelBuilder(const QString &name)
{
    PluginManager *pluginManager = PluginManager::instance();
    foreach (QObject *obj, pluginManager->interfaceObjects("IDatabaseModelBuilder")) {
        IDatabaseModelBuilder *iBuilder = qobject_cast<IDatabaseModelBuilder*>(obj);
        if (iBuilder->modelBuilderName() == name)
            return iBuilder;
    }

    return NULL;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(databasemodel, DatabaseModel)
#endif


