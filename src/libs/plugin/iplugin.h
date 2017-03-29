#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtPlugin>
#include <QStringList>
#include <QIcon>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include "pluginmanager.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QString;
class QStringList;
class QObject;
QT_END_NAMESPACE

#define PLUG_INTERFACES(x) QStringList interfaces(){return QString(#x).split(" ");}
#undef  Q_INTERFACES
#define Q_INTERFACES(x) PLUG_INTERFACES(x)

//! Класс плагина
/*! Этот класс предназначен для создания пользовательского
    плагина путем наследования данного класса. Работа с
    плагинами производится при помощи класса PluginManager
    \n
    Пример: \n
    \b exampleplug.h
    \code
        #ifndef PLUG_EXAMPLE_H
        #define PLUG_EXAMPLE_H

        #include <plugin/iplugin.h>
        #include <itreedockwidget.h>

        class ExamplePlug:
                public QObject,
                public IPlugin
        {
            Q_OBJECT
            Q_INTERFACES(IPlugin)
            Q_PLUGIN_METADATA(
                 IID "com.RTPTechGroup.Raindrop.Example" FILE "example.json")
        #endif

        public:

            ExamplePlug(QObject *parent = 0);

        // IPlugin
            //! Получение имени плагина
            QString name() {return tr("Пример плагина");}

            //! Получение иконки плагина
            QIcon icon() {return QIcon(":/example");}

            //! Описание продукта
            QString product() {return tr("");}

            //! Авторские права
            QString copyright() {return tr("");}

            //! Описание плагина
            QString descript() {return tr("");}

            //! Категория в которой состоит плагин
            QString category() {return tr("");}

            //! Версия плагина
            QString version() {return tr("");}

            //! Производитель плагина
            QString vendor() {return tr("");}

            MainWindow *window;
        };
        #endif
    \endcode
    \b exampleplug.cpp
    \code
        #include "exampleplug.h"

        ExamplePlug::ExamplePlug(QObject *parent):
            QObject(parent), IPlugin("IMainWindow ITreeDockWidget")
        {
            // Получение интерфейса ITreeDockWidget
            PluginManager *pluginManager = PluginManager::instance();
            ITreeDockWidget *dockWidget
                      = pluginManager->interfaceObject<ITreeDockWidget*>(
                          "ITreeDockWidget");
        }

        void ExamplePlug::readSettings()
        {
            window->resize(settings->value("size", QSize(687, 582)).toSize());
        }

        void ExamplePlug::writeSettings()
        {
            settings->setValue("size", window->size());
        }
        Q_EXPORT_PLUGIN2(example, ExamplePlug)

    \endcode
*/

class PLUGINLIB IPlugin
{

public:    
    //! Конструктор плагина
    explicit IPlugin(const QString &depInterfaces);

    //! Деструктор плагина
    virtual ~IPlugin();

    //! Получение экземпляра
    virtual QObject *instance() {return dynamic_cast<QObject *>(this);}

    //! Получение экземпляра с приведением типа
    template<typename T> T instance() {return dynamic_cast<T>(this);}

    //! Получение определенных интерфейсов
    virtual QStringList interfaces() = 0;

    //! Список зависимостей плагина
    /*! Список плагинов от которых зависит текущий плагин
    */
    const QStringList& depModulList() const;

    //! Устанавливает ссылку на объект для сохранения настроек
    virtual void setSettings(QSettings *settings);

    //! Получает ссылку на объект для сохранения настроек
    virtual QSettings *settings();

    //! Получение имени плагина
    /*! Получение наименования плагина для
        отображения
    */
    virtual QString name() = 0;

    //! Получение иконки плагина
    /*! Получение иконки плагина для
        отображения
    */
    virtual QIcon icon() = 0;

    //! Описание плагина
    /*! Получение описания плагина для
        отображения
    */
    virtual QString descript() = 0;

    //! Категория в которой состоит плагин
    /*! Категория плагинов предназначена для группировки плагинов
     *  по смыслу в одну группу.
     */
    virtual QString category() = 0;

    //! Версия плагина
    virtual QString version() = 0;

    //! Производитель плагина
    virtual QString vendor() = 0;

    //! Описание продукта
    virtual QString product() = 0;

    //! Авторские права
    virtual QString copyright() = 0;

private:
    //! Настройки
    QSettings *m_settings;

    //! Список зависимостей плагина
    QStringList m_depModulList;
};

QT_BEGIN_NAMESPACE

#define IPlugin_iid  "com.RTPTechGroup.Raindrop.IPlugin"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

QT_END_NAMESPACE

#endif
