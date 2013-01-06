#include "pluginmanager.h"

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

IPlugin *PluginManager::getPlugin(QString className)
{
    foreach(IPlugin* pluginItem, plugList)
        if (className.isEmpty() || pluginItem->instance()->inherits(className.toLatin1().data()))
            return pluginItem;
    return 0;
}

QString PluginManager::pluginClass(IPlugin *plug) const
{
    return plugList.key(plug);
}

PluginManager::~PluginManager()
{
    foreach (IPlugin* corePlugin,plugList)
        if (corePlugin)
            delete corePlugin;
}

QSettings* PluginManager::settings() const
{
    return m_settings;
}

// Инициализация плагина
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
        if (!initPlugin(getPlugin(depPlugin)))
        {
            delete plug;
            return false;
        }

    if (m_settings)
    {
        plug->setSettings(m_settings);
        m_settings->beginGroup(plug->instance()->metaObject()->className());
        plug->readSettings();
        m_settings->endGroup();
    }
    plug->initialize();

    emit showMessage(tr("Инициализирован плагин: %1").arg(plug->name()));
    qDebug() << "Init plugin:" << plug->name();
    plug->state = IPlugin::Init;
    return true;
}

QList<IPlugin *> PluginManager::dependentPlugins(IPlugin *plug)
{
    QList<IPlugin *> pluginList;
    foreach (IPlugin* depPlugin,plugList){
        if (depPlugin && depPlugin!=plug)
            foreach (QString depPluginName,depPlugin->depModulList)
                if(plug == plugList.value(depPluginName))
                    pluginList << depPlugin;
    }
    return pluginList;
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
                plugList[plugin->objectName()] = corePlugin;
                emit showMessage(tr("Загружен плагин: %1").arg(corePlugin->name()));
                qDebug()<<"Load plugin: "<<corePlugin->name();
            }else
                qDebug()<<"Error load plugin" << loader.errorString();
        }
        else
            qDebug()<<"Error load plugin" << loader.errorString();
    }

    // Инициализация
    foreach (IPlugin* corePlugin,plugList)
    {
        initPlugin(corePlugin);
    }
    QDir::setCurrent(qApp->applicationDirPath());
}

void PluginManager::setSettings(QSettings *s)
{
    m_settings = s;
}

void PluginManager::removePlugin(QObject *obj)
{
    plugList.remove(obj->objectName());
    qDebug() << "Remove plugin:" << obj->objectName();
}
