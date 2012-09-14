#include "pluginmanager.h"
#include "plugincollection.h"
#include "objectcollection.h"
#include <QtGui>
#include <QApplication>
#include <pluginviewdlg.h>
#include <common.h>

QPointer<PluginManager> PluginManager::m_instance = NULL;

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
    m_settings = NULL;
    m_pluginViewDlg = NULL;
    m_pluginCollection = new PluginCollection(this);
    m_objectCollection = new ObjectCollection(this);

    connect(m_pluginCollection, SIGNAL(showMessage(QString, int, QColor)),
                        this , SIGNAL(showMessage(QString, int, QColor)));
}

PluginManager::~PluginManager()
{
    writeSettings();
    unloadPlugins();

    m_instance = NULL;
    DELETE_OBJECT(m_pluginCollection);
    DELETE_OBJECT(m_objectCollection);
    DELETE_OBJECT(m_pluginViewDlg);
}

//! Получение экземпляра менеджера плагинов
QPointer<PluginManager> PluginManager::instance()
{
    if (!m_instance)
    {
        m_instance = new PluginManager();
    }
    return m_instance;
}

/*!
 * \brief Освобождение экземпляра
 */
void PluginManager::release()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = 0;
    }
}

/*! Коллекция плагинов
 */
QPointer<PluginCollection> PluginManager::pluginCollection()
{
    return m_pluginCollection;
}

/*! Коллекция объектов
  */
QPointer<ObjectCollection> PluginManager::objectCollection()
{
    return m_objectCollection;
}

// Settings
void PluginManager::setSettings(QPointer<QSettings> settings)
{
    m_settings = settings;
}

QPointer<QSettings> PluginManager::settings()
{
    if (!m_settings)
        m_settings = new QSettings();

     return m_settings;
}

/*!
 *  \brief Чтение настроек
 */
void PluginManager::readSettings()
{
    emit showMessage(tr("Чтение настроек системы расширений... "));
}

/*!
 * \brief Запись настроек
 */
void PluginManager::writeSettings()
{
}

//! \brief Добавления объекта в общий список менеджера плагинов
/*! \param obj Объект для добавления в общий список менеджера плагинов
 *
 *   При добавлении объекта в общий список менеджера плагинов
 *   этот объект можно получить из вне при помощи менеджера плагинов
 *   \sa getObjectByName()и getObjectByClassName()
 */
void PluginManager::addObject(QPointer<QObject> obj)
{
    m_objectCollection->addObject(obj);
}

/*! \param obj Объект для удаления из общего списка менеджера плагинов.
    В результате удаления объекта из общего списка менеджера плагинов
    он становится не доступен для доступа из вне при помощи менеджера плагинов.
    \sa getObjectByName()и getObjectByClassName()
*/
void PluginManager::removeObject(QPointer<QObject> obj)
{
    m_objectCollection->removeObject(obj);
}

/*! \param name Имя объекта
    Предназначено для получения объекта из вне из общего списка
    менеджера плагинов по его имени
    \sa addObject(),removeObject()
*/
QPointer<QObject> PluginManager::objectByName(const QString &name) const
{
    return m_objectCollection->getObjectByName(name);
}

/*! \brief Предоставляет объект по имени его класса
 *  \param className Имя класса
 *  Предназначено для получения объекта из вне из общего списка
 *  менеджера плагинов по его имени класса
 *  \sa addObject(),removeObject()
*/
QPointer<QObject> PluginManager::objectByClassName(const QString &className) const
{
    return m_objectCollection->getObjectByClassName(className);
}

/*!
 *  \brief Предоставляет список путий поиска плагинов.
 */
QStringList PluginManager::pluginPaths() const
{
    return m_pluginPaths;
}

/*!
 * \brief Устанавливает пути поиска плагинов
 */
void PluginManager::setPluginPaths(const QStringList &paths)
{
    m_pluginPaths = paths;
}

/*! \brief Предоставляет интерфейс доступа к экземпляру плагина по его имени.
 * \param name Имя плагина
 */
IPlugin * PluginManager::pluginByName(const QString &name) const
{
    return m_pluginCollection->pluginByName(name);
}

/*!
 *  \brief Предоставляет интерфейс доступа к экземпляру плагина по имени его класса.
 * \param className Имя класса плагина
 */
IPlugin * PluginManager::pluginByClassName(const QString &className) const
{
    return m_pluginCollection->pluginByClassName(className);
}

/*! \brief Функция создает окно для отображения информации о плагинах
 *  \param parent родительское окно которому бедеи принадлежать созданое окно просмотра
 */

void PluginManager::showPluginView(QWidget *parent)
{
    if (m_pluginViewDlg.isNull())
    {
        m_pluginViewDlg = new PluginViewDlg(parent);        
        m_pluginModel.setPlugins(pluginCollection()->plugins());
        m_pluginViewDlg->setModel(&m_pluginModel);
    }
    m_pluginViewDlg->show();
}

/*!
 *  \brief Функция производит поиск плагина на диске в каталоге
 *   plugins запущенного приложения и загружает его
 */
bool PluginManager::loadPlugins()
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
        QPointer<QObject> pluginObject = loader.instance();

        if (!pluginObject.isNull())
        {
            IPlugin * plugin = qobject_cast<IPlugin*>(pluginObject);
            if(plugin != 0)
            {
                plugin->setSettings(settings());
                pluginCollection()->addPlugin(plugin);

                emit showMessage(tr("Чтение плагина: %1").arg(plugin->name()));

                #ifdef QT_DEBUG
                    qDebug() << tr("Плагин %1 успешно прочитан").arg(plugin->name());
                #endif
            }
            else
            {
                #ifdef QT_DEBUG
                qDebug() << tr("Ошибка чтения плагина: Файл %1 не является корректным модулем IPlugin").arg(pluginsDir.absoluteFilePath(fileName));
                #endif
            }
        }
        else
        {
            #ifdef QT_DEBUG
                qDebug() << tr("Ошибка чтения плагина: %1 ").arg(loader.errorString());
            #endif
        }
    }
    QDir::setCurrent(qApp->applicationDirPath());
    return pluginCollection()->initializePlugins();
}

/*!
 *  \brief Функция производит выгрузку плагинов
 */
void PluginManager::unloadPlugins()
{
     pluginCollection()->releasePlugins();
     pluginCollection()->deletePlugins();
}

