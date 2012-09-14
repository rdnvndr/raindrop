#include "plugincollection.h"
#include <QDebug>

PluginCollection::PluginCollection(QObject *parent) :
    QObject(parent)
{
}

PluginCollection::~PluginCollection()
{
}

/*!
 *  \brief Добавляет плагин в колекцию.
 */
void PluginCollection::addPlugin(IPlugin * plugin)
{
    m_plugins.insert(plugin->name(), plugin);
}

/*!
 *  \brief Удаляет плагин из колекции. При этом сам плагин не удаляется.
 */
void PluginCollection::removePlugin(IPlugin * plugin)
{
    m_plugins.remove(plugin->name());
}

/*!
 *  \brief Получение плагина по имени.
 */
IPlugin* PluginCollection::pluginByName(const QString &name) const
{
    QHash<QString, IPlugin*>::const_iterator plugin =  m_plugins.find(name);

    if (plugin != m_plugins.end()) {
        return plugin.value();
    }
    else {
        #ifdef QT_DEBUG
            qDebug() << tr("Плагина с именем \"%1\" не существует").arg(name);
        #endif
        return 0;
    }
}

/*!
 *  \brief Плучение плагина по имени его класса.
 */
IPlugin* PluginCollection::pluginByClassName(const QString &className) const
{
    const QByteArray baClassName = className.toUtf8();
    QHashIterator<QString, IPlugin*> obj(m_plugins);
    while (obj.hasNext())
    {
        obj.next();
        if (obj.value()->inherits(baClassName.constData()))
        {
            return obj.value();
        }
    }

    return 0;
}

/*!
 *  \brief Контейнер плагинов
 */
QHash<QString, IPlugin*> PluginCollection::plugins() const
{
    return m_plugins;
}

/*!
 *  \brief Инициализация плагинов
 */
bool PluginCollection::initializePlugins()
{
    foreach (IPlugin* plugin, m_plugins)
    {
        initializePlugin(plugin);
    }
    return true;
}

/*!
 *  \brief Инициализация плагина (рекурсивная)
 */
bool PluginCollection::initializePlugin(IPlugin* plugin)
{
    if(plugin == 0) return false;

    emit showMessage(tr("Инициализация плагина: %1").arg(plugin->name()));

    if (plugin->state == IPlugin::Init)
    {
        return true;
    }
    else if (plugin->state  ==  IPlugin::Error)
    {
        return false;
    }
    else if (plugin->state  ==  IPlugin::Lock)
    {
        QMessageBox::critical(NULL, tr("Ошибка"), tr("Рекурсивная зависимость на плагин %1").arg(plugin->name()));
        plugin->state = IPlugin::Error;
        plugin->setError();
        plugin->setErrorString(tr("Рекурсивная зависимость"));
        return false;
    }

    plugin->state = IPlugin::Lock;

    // Инициализация зависимостей
    foreach (QString pluginName, plugin->dependence())
    {
        IPlugin * dependPlugin = pluginByName(pluginName);

        if(dependPlugin != 0)
        {
            if(!initializePlugin(dependPlugin))
            {
                plugin->state = IPlugin::Error;
                plugin->setError();
                plugin->setErrorString(dependPlugin->errorString());

                #ifdef QT_DEBUG
                    qDebug() << tr("При инициализации плагина %1 произошла ошибка").arg(plugin->name());
                #endif
                return false;
            }
            dependPlugin->addClient(plugin->name());
        }
        else
        {
            plugin->state = IPlugin::Error;
            plugin->setError();
            plugin->setErrorString(tr("Попытка инициализации не существующей зависимости %1").arg(pluginName));
            #ifdef QT_DEBUG
                qDebug() << tr("При инициализации плагина %1 произошла ошибка").arg(plugin->name());
            #endif
            return false;
        }
    }

    // Чтение настроек плагина
    plugin->settings()->beginGroup(plugin->metaObject()->className());
    plugin->readSettings();
    plugin->settings()->endGroup();

    bool initResult = plugin->initialize();
    plugin->state = IPlugin::Init;

    #ifdef QT_DEBUG
        qDebug()<< tr("Плагин %1 успешно инициализирован").arg(plugin->name());
    #endif

    return initResult;
}

/*!
 * \brief Освобождение ресурсов плагинов
 */
bool PluginCollection::releasePlugins()
{
    // Вызываем вункцию освобождения ресурсов плагинов
    foreach (IPlugin* plugin, m_plugins)
    {
        releasePlugin(plugin);
    }

    return true;
}

/*!
 * \brief Освобождение плагина (рекурсивная)
 */
bool PluginCollection::releasePlugin(IPlugin* plugin)
{
    if(plugin == 0) return false;

    if (plugin->state  == IPlugin::NoInit)
    {
        return true;
    }
    else if ((plugin->state ==  IPlugin::Lock) || (plugin->state == IPlugin::Error))
    {
        return false;
    }

    plugin->state = IPlugin::Lock;

    // Освобождение зависимостей
    foreach (QString pluginName, plugin->clients())
    {
        IPlugin *dependPlugin = pluginByName(pluginName);
        if(dependPlugin != 0 )
        {
            if(dependPlugin->state != IPlugin::Error)
            {
                if (!releasePlugin(pluginByName(pluginName)))
                {
                    plugin->state = IPlugin::Error;

                    #ifdef QT_DEBUG
                        qDebug() << tr("При освобождении плагина %1 произошла ошибка").arg(plugin->name());
                    #endif

                    return false;
                }
            }
            else
            {
                #ifdef QT_DEBUG
                    qDebug() << tr("При освобождении плагин  %1 с ошибкой пропущен").arg(dependPlugin->name());
                #endif
            }
        }
    }

    // Чтение настроек плагина
    plugin->settings()->beginGroup(plugin->metaObject()->className());
    plugin->writeSettings();
    plugin->settings()->endGroup();

    plugin->release();
    plugin->state = IPlugin::NoInit;

    #ifdef QT_DEBUG
        qDebug()<< tr("Плагин %1 успешно освобожден").arg(plugin->name());
    #endif

    return true;
}

/*!
 *  \brief Удаление всех плагинов из колекции и освобождение памяти.
 */
void PluginCollection::deletePlugins()
{
    qDeleteAll(m_plugins);
    m_plugins.clear();
}
