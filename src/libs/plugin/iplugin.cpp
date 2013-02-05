#include "iplugin.h"

IPlugin::IPlugin(QString depInterfaces)
{
    depModulList = depInterfaces.split(" ");
    PluginManager *pluginManager = PluginManager::instance();
    foreach (QString depPlugin, depModulList) {
        bool existPlugin = true;
        do {   
            if (pluginManager->interfaceObject(depPlugin) || depPlugin.isEmpty())
                break;
            existPlugin = pluginManager->nextLoadPlugin();
        } while (existPlugin);

        // Интерфейс не найден
        if (!existPlugin) {
            qDebug() << "Interface not found: " << depPlugin;
            throw 1;
        }
    }
    if (pluginManager->settings())
        setSettings(pluginManager->settings());
}

IPlugin::~IPlugin()
{
    foreach (IPlugin* plug,PluginManager::instance()->dependentPlugins(this))
        if (plug)
            delete plug;
}

void IPlugin::setSettings(QSettings *s){
    m_settings = s;
}

QSettings* IPlugin::settings()
{
    return m_settings;
}
