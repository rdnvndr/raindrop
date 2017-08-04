#include "pluginloadlog.h"

PluginLoadLog::PluginLoadLog(QObject *parent) :
    QObject(parent)
{

}

PluginLoadLog::~PluginLoadLog()
{
    m_loadedPlugins.clear();
}

QStringList PluginLoadLog::loadedPlugins()
{
    return m_loadedPlugins;
}

void PluginLoadLog::clearLoadedPlugins()
{
    m_loadedPlugins.clear();
}

void PluginLoadLog::loadPlugin(QObject *plugin)
{
    m_loadedPlugins.append(plugin->objectName());
}
