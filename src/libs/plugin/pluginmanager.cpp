#include "pluginmanager.h"

#include <QRegExp>

#include "clogging.h"

namespace RTPTechGroup {
namespace Plugin {

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
    m_settings = NULL;
    m_lockFiles = NULL;


#ifndef PLUGIN_DIR
    m_pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/");
#else
    m_pluginsDir = QDir(qApp->applicationDirPath() + "/" + PLUGIN_DIR);
#endif
}

PluginManager *PluginManager::m_instance = 0;

PluginManager *PluginManager::instance()
{
    return m_instance;
}

QObject *PluginManager::interfaceObject(QString interfaceName)
{
    return m_interfaces.value(interfaceName, NULL);
}

QList<QObject *> PluginManager::interfaceObjects(QString interfaceName)
{
    return m_interfaces.values(interfaceName);
}

PluginManager::~PluginManager()
{
    QObject *plug = interfaceObject("IPlugin");
    while (plug) {
        delete plug;
        plug = interfaceObject("IPlugin");
    }
    delete [] m_lockFiles;
}

QSettings *PluginManager::settings() const
{
    return m_settings;
}

QList<IPlugin *> PluginManager::dependPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    if (plugin)
        foreach (const QString &depInterfaceName, plugin->depModulList())
            foreach (QObject *objInterfacePlugin, interfaceObjects(depInterfaceName)) {
                IPlugin *interfacePlugin = qobject_cast<IPlugin *>(objInterfacePlugin);
                if (interfacePlugin)
                    pluginList[objInterfacePlugin->objectName()] = interfacePlugin;
            }
    return pluginList.values();
}

QList<IPlugin *> PluginManager::dependentPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    foreach (QObject *objPlug, m_interfaces.values("IPlugin")) {
        IPlugin *plug = qobject_cast<IPlugin *>(objPlug);
        if (plug && plug!=plugin)
            foreach (IPlugin *interfacePlugin,dependPlugins(plug))
                if (plugin == interfacePlugin)
                    pluginList[objPlug->objectName()] = plug;
    }
    return pluginList.values();
}

bool PluginManager::loadPlugins()
{
    if (!m_pluginsDir.exists())
    {
        qCWarning(lcPlugin) << tr("Не найден каталог с модулями ")
                               + m_pluginsDir.absolutePath();
        return false;
    }
    m_fileList = m_pluginsDir.entryList(QDir::Files);

    qint32 count = m_fileList.count();
    m_lockFiles = new bool[count];
    for (qint32 i = 0; i < count; ++i) m_lockFiles[i] = false;

    nextLoadPlugins();
    emit endLoadingPlugins();

    return true;
}

bool PluginManager::nextLoadPlugins(QString iid)
{
    bool result = false;
    for (qint32 fileNum = m_fileList.count()-1; fileNum >= 0 ; --fileNum)
    {
        if (!m_lockFiles[fileNum]) {
            m_lockFiles[fileNum] = true;
            bool isLoad = loadPlugin(m_fileList.at(fileNum), iid);
            if (!isLoad)
                m_lockFiles[fileNum] = false;
            result = result || isLoad;
        }
    }
    return result;
}

bool PluginManager::loadPlugin(QString fileName, QString iid)
{
    fileName = m_pluginsDir.absoluteFilePath(fileName);
    if (!QLibrary::isLibrary(fileName)) {
        qCWarning(lcPlugin) << tr("Файл не является модулем ") + fileName;
        return false;
    }

    QPluginLoader loader(fileName);

    QString plugIid = loader.metaData().value("IID").toString();
    QRegExp checkIid("\\."+iid+"([\\.\\][0-9]+.?[0-9]*)?");
    if (!plugIid.isEmpty() && !plugIid.contains(checkIid)) {
        return false;
    }

    QObject *plugin = loader.instance();
    if (plugin)
    {
        IPlugin *corePlugin = qobject_cast<IPlugin*>(plugin);
        if (corePlugin){
            connect(plugin, &QObject::destroyed,
                    this, &PluginManager::removePlugin);
            plugin->setObjectName(plugin->metaObject()->className());

            foreach (const QString &interface, corePlugin->interfaces())
                m_interfaces.insert(interface, plugin);

            emit loadedPlugin(plugin);
            qCDebug(lcPlugin) << tr("Загружен модуль ") + plugin->objectName();
            return true;
        } else
            delete plugin;
    }
    qCWarning(lcPlugin) << loader.errorString();
    return false;
}

void PluginManager::setSettings(QSettings *s)
{
    m_settings = s;
}

void PluginManager::removePlugin(QObject *obj)
{
    foreach (const QString &interface, m_interfaces.keys())
        foreach (QObject *plug,m_interfaces.values(interface))
            if (plug == obj) {
                m_interfaces.remove(interface, plug);
                qCDebug(lcPlugin) << tr("Очищен модуль ") + plug->objectName();
            }
    qCDebug(lcPlugin) << tr("Выгружен модуль ") + obj->objectName();
    emit removedPlugin(obj);
}

QDir PluginManager::pluginsDir() const
{
    return m_pluginsDir;
}

void PluginManager::setPluginsDir(const QDir &pluginsDir)
{
    m_pluginsDir = pluginsDir;
}

}}
