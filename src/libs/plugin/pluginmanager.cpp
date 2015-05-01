#include "pluginmanager.h"
#include <typeinfo>

namespace RTPTechGroup {
namespace Plugin {

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
    m_settings = NULL;
    m_currentFile = 0;
}

PluginManager *PluginManager::m_instance = 0;

PluginManager *PluginManager::instance()
{
    return m_instance;
}

QObject *PluginManager::interfaceObject(QString interfaceName)
{
    if (m_interfaces.count(interfaceName) > 0)
        return m_interfaces.values(interfaceName).takeFirst();
    return 0;
}

QList<QObject *> PluginManager::interfaceObjects(QString interfaceName)
{
    return m_interfaces.values(interfaceName);
}

PluginManager::~PluginManager()
{
    QObject* plug = interfaceObject("IPlugin");
    while (plug) {
        delete plug;
        plug = interfaceObject("IPlugin");
    }
}

QSettings* PluginManager::settings() const
{
    return m_settings;
}

QList<IPlugin *> PluginManager::dependPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    if (plugin)
        foreach (QString depInterfaceName,plugin->depModulList())
            foreach (QObject* objInterfacePlugin,interfaceObjects(depInterfaceName)) {
                IPlugin *interfacePlugin = qobject_cast<IPlugin *>(objInterfacePlugin);
                if (interfacePlugin)
                    pluginList[objInterfacePlugin->objectName()] = interfacePlugin;
            }
    return pluginList.values();
}

QList<IPlugin *> PluginManager::dependentPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    foreach (QObject* objPlug, m_interfaces.values("IPlugin")) {
        IPlugin *plug = qobject_cast<IPlugin *>(objPlug);
        if (plug && plug!=plugin)
            foreach (IPlugin* interfacePlugin,dependPlugins(plug))
                if (plugin == interfacePlugin)
                    pluginList[objPlug->objectName()] = plug;
    }
    return pluginList.values();
}

void PluginManager::loadPlugins()
{
    m_pluginsDir = QDir(qApp->applicationDirPath());
    if (!m_pluginsDir.cd("plugins"))
    {
        QMessageBox::critical(NULL, tr("Ошибка"), tr("Каталог с модулями не найден"));
        return;
    }

    // Загрузка файлов
    QDir::setCurrent(qApp->applicationDirPath()+"\\"+"plugins");
    m_listFiles = m_pluginsDir.entryList(QDir::Files);
    for (m_currentFile=0; m_currentFile < m_listFiles.count(); m_currentFile++)
        loadPlugin(m_listFiles.at(m_currentFile));

    QDir::setCurrent(qApp->applicationDirPath());
    emit endLoadingPlugins();
}

bool PluginManager::nextLoadPlugin() {

    if (m_currentFile <  m_listFiles.count()-1) {
        m_currentFile++;
        loadPlugin(m_listFiles.at(m_currentFile));
        return true;
    }
    return false;
}

bool PluginManager::loadPlugin(QString fileName)
{
    if (!QLibrary::isLibrary(fileName))
        return false;
    try {
        QPluginLoader loader(m_pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();
        if (plugin)
        {
            IPlugin* corePlugin = qobject_cast<IPlugin*>(plugin);
            if (corePlugin){
                connect(plugin,SIGNAL(destroyed(QObject*)),this,SLOT(removePlugin(QObject*)));
                plugin->setObjectName(plugin->metaObject()->className());

                foreach (const QString &interface, corePlugin->interfaces())
                    m_interfaces.insert(interface, plugin);

                emit loadedPlugin(plugin);
                qDebug()<<"Load plugin: "<<corePlugin->name();
                return true;
            }else {
                qDebug()<<"Error load plugin" << loader.errorString();
                delete plugin;
                return false;
            }
        } else {
            qDebug()<<"Error load plugin" << loader.errorString();
            return false;
        }
    } catch (int e) {
        Q_UNUSED(e)
        return false;
    }
}

void PluginManager::setSettings(QSettings *s)
{
    m_settings = s;
}

void PluginManager::removePlugin(QObject *obj)
{
    foreach (QString interface, m_interfaces.keys())
        foreach (QObject *plug,m_interfaces.values(interface))
            if (plug == obj) {
                m_interfaces.remove(interface,plug);
                qDebug() << "clean:" << plug->objectName();
            }
    emit removedPlugin(obj);
    qDebug() << "Remove plugin:" << obj->objectName();
}

}}
