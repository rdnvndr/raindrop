#include "pluginmanager.h"
#include <typeinfo>

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
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

bool PluginManager::initPlugin(IPlugin* plug)
{
    if (!plug)
        return false;

    if (plug->state.testFlag(IPlugin::Init))
        return true;
    else if (plug->state.testFlag(IPlugin::Lock))
    {
        QMessageBox::critical(NULL, tr("Ошибка"), tr("Рекурсивная зависимость плагинов"));
        return false;
    }

    plug->state = IPlugin::Lock;

    foreach (QString depPlugin,plug->depModulList)
        foreach (QObject* interfacePlugin, interfaceObjects(depPlugin))
            if (!initPlugin(qobject_cast<IPlugin *>(interfacePlugin)))
            {
                delete plug;
                return false;
            }

    plug->initialize();

    emit showMessage(tr("Инициализирован плагин: %1").arg(plug->name()));
    qDebug() << "Init plugin:" << plug->name();
    plug->state = IPlugin::Init;
    return true;
}

QList<IPlugin *> PluginManager::dependPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    if (plugin)
        foreach (QString depInterfaceName,plugin->depModulList)
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
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    if (!pluginsDir.cd("plugins"))
    {
        QMessageBox::critical(NULL, tr("Ошибка"), tr("Каталог с модулями не найден"));
        return;
    }

    // Загрузка файлов
    QDir::setCurrent(qApp->applicationDirPath()+"\\"+"plugins");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject* plugin = loader.instance();

        if (plugin)
        {
            IPlugin* corePlugin = qobject_cast<IPlugin*>(plugin);
            if (corePlugin){
                connect(plugin,SIGNAL(destroyed(QObject*)),this,SLOT(removePlugin(QObject*)));
                plugin->setObjectName(plugin->metaObject()->className());

                foreach (const QString &interface, corePlugin->interfaces())
                    m_interfaces.insert(interface, plugin);

                if (m_settings)
                    corePlugin->setSettings(m_settings);

                emit showMessage(tr("Загружен плагин: %1").arg(corePlugin->name()));
                qDebug()<<"Load plugin: "<<corePlugin->name();
            }else {
                qDebug()<<"Error load plugin" << loader.errorString();
                delete plugin;
            }
        } else
            qDebug()<<"Error load plugin" << loader.errorString();
    }

    // Инициализация
    foreach (QObject* corePlugin,m_interfaces.values("IPlugin"))
        initPlugin(qobject_cast<IPlugin *>(corePlugin));

    QDir::setCurrent(qApp->applicationDirPath());
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

    qDebug() << "Remove plugin:" << obj->objectName();
}
