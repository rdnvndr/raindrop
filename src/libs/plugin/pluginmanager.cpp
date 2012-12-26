#include "pluginmanager.h"
#include <QMessageBox>

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

QObject *PluginManager::getObjectByName(const QString &name) const
{
    return m_objects[name];
}

QObject *PluginManager::getObjectByClassName(const QString &className) const
{
    const QByteArray ba = className.toUtf8();

    foreach (QObject *obj, m_objects) {
        if (obj->inherits(ba.constData()))
            return obj;
    }
    return 0;
}

IPlugin *PluginManager::getPlugin(const QString &className) const
{
    return plugList.value(className);
}

QString PluginManager::pluginClass(IPlugin *plug) const
{
    return plugList.key(plug);
}

void PluginManager::addObject(QObject *obj)
{
    m_objects[obj->objectName()] = obj;
}

void PluginManager::removeObject(QObject *obj)
{
   m_objects.remove(m_objects.key(obj));
}

PluginManager::~PluginManager()
{
    foreach (QString str, plugList.keys()){
        IPlugin* corePlugin = plugList.value(str);
        if (corePlugin)
            delete corePlugin;
    }
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
    {
        if (!initPlugin(getPlugin(depPlugin)))
        {
            delete plug;
            return false;
        }
    }

    if (m_settings)
    {
        plug->setSettings(m_settings);
        m_settings->beginGroup(plug->metaObject()->className());
        plug->readSettings();
        m_settings->endGroup();
    }
    plug->initialize();
    emit showMessage(tr("Инициализирован плагин: %1").arg(plug->name()));
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
                plugList[plugin->metaObject()->className()] = corePlugin;
                emit showMessage(tr("Загружен плагин: %1").arg(corePlugin->name()));
                qDebug()<<"Load plugin "<<corePlugin->name();
            }else
                qDebug()<<"Error load plugin" << loader.errorString();
        }
        else
            qDebug()<<"Error load plugin" << loader.errorString();
    }

    // Инициализация
    foreach (IPlugin* corePlugin,plugList)
        initPlugin(corePlugin);
    QDir::setCurrent(qApp->applicationDirPath());
}

void PluginManager::setSettings(QSettings *s)
{
    m_settings = s;
}

void PluginManager::removePlugin(QObject *obj)
{
    QString className = pluginClass(static_cast<IPlugin*>(obj));
    qDebug() << "Remove plugin:" << className;
    plugList.remove(className);
}
