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
class PluginManager;
class QObject;
QT_END_NAMESPACE

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


        #if QT_VERSION >= 0x050000
            Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.Example" FILE "example.json")
        #endif

        public:

            ExamplePlug(QObject *parent = 0);

        // IPlugin
            //! Инициализация плагина
            bool initialize();

            //! Освобожение плагина
            bool release();

            //! Чтение настроек
            void readSettings();

            //! Запись настроек
            void writeSettings();

            //! Получение экземпляра
            QObject *instance() { return this; }

            MainWindow *window;
        };
        #endif
    \endcode
    \b exampleplug.cpp
    \code
        #include "exampleplug.h"

        ExamplePlug::ExamplePlug(QObject *parent):
            QObject(parent)
        {
            setName(tr("Пример плагина"));
            setIcon(QIcon(":/example"));
            addDepend("ITreeDockWidget");
            window = new QMainWindow();
        }


        bool ExamplePlug::initialize()
        {
            // Получение интерфейса ITreeDockWidget
            PluginManager* pluginManager = PluginManager::instance();
            ITreeDockWidget* dockWidget = qobject_cast<ITreeDockWidget*>(
                        pluginManager->getPlugin("ITreeDockWidget")->instance());
        }

        bool ExamplePlug::release()
        {
            return true;
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
class IPlugin
{

public:

    //! Перечисление состояний плагина
    /*! Описывает в каком состоянии находится плагин на данный момент
        Используется при инициализации плагина
    */
    enum StateFlag {
        NoInit = 0x0,   //!< Плагин не инициализирован
        Init = 0x1,     //!< Плагин инициализирован
        Lock = 0x2      //!< Плагин заблокирован
    };
    Q_DECLARE_FLAGS(State, StateFlag)

    //! Получение экземпляра
    virtual QObject *instance() = 0;

    //! Состояние плагина
    IPlugin::State state;

    //! Список зависимостей плагина
    /*! Список плагинов от которых зависит текущий плагин
    */
    QStringList depModulList;

    //! Конструктор плагина
    explicit IPlugin();

    //! Деструктор плагина
    virtual ~IPlugin();


    //! Устанавливает имя плагина
    /*! Устанавливает наименование плагина для
        отображения
    */
    virtual void setName(QString name);

    //! Получение имени плагина
    /*! Получение наименования плагина для
        отображения
    */
    virtual QString name();

    //! Устанавливает иконку для плагина
    /*! Устанавливает иконку плагина для
        отображения
    */
    virtual void setIcon(QIcon icon);


    //! Получение иконки плагина
    /*! Получение иконки плагина для
        отображения
    */
    virtual QIcon icon();

    //! Инициализация плагина
    /*! Производит инициализацию текущего плагина.
        Вызывается после загрузки всех модулей
    */
    virtual bool initialize() = 0;

    //! Освобождение плагина
    /*! Производит деинициализацию плагина
     *  Оставлен для совместимости с extensionsystem
     */
    virtual bool release() = 0;

    //! Добавление зависимостей от других плагинов
    /*! При вызове данного метода добавляются зависимости
        от других плагинов. При инициализации плагинов определяются
        зависимости и в случае если зависимый плагин не инициализирован
        инициализируется сначало зависимый плагин
        \sa initPlug()
    */
    virtual void addDepend(QString s);

    //! Устанавливает ссылку на объект для сохранения настроек
    virtual void setSettings(QSettings *settings);

    //! Получает ссылку на объект для сохранения настроек
    virtual QSettings *settings();

    //! Чтение настроек
    /*! Метод предназначен для чтения настроек
        плагина и их установки
    */
    virtual void readSettings();

    //! Запись настроек
    /*! Метод предназначен для записи настроек
        плагина
    */
    virtual void writeSettings();

    //! Описание плагина
    /*! Получение описания плагина для
        отображения
    */
    virtual QString descript() const;

    //! Категория в которой состоит плагин
    /*! Категория плагинов предназначена для группировки плагинов
     *  по смыслу в одну группу.
     */
    virtual QString category() const;

    //! Версия плагина
    virtual QString version() const;

    //! Производитель плагина
    virtual QString vendor() const;

    //! Установка описания плагина
    /*! Устанавливает описание плагина для
        отображения
    */
    virtual void setDescript(QString descript);

    //! Установить категорию плагина
    virtual void setCategory(const QString category);

    //! Установить версию плагина
    virtual void setVersion(const QString version);

    //! Установить производителя плагина
    virtual void setVendor(const QString vendor);

private:
    //! Имя плагина (описание)
    QString m_PlugName;
    //! Иконка плагина
    QIcon m_PlugIcon;
    //! Описание плагина
    QString m_descript;
    //! Категория в которой состоит плагин
    QString m_category;
    //! Версия плагина
    QString m_version;
    //! Производитель плагина
    QString m_vendor;
    //! Настройки
    QSettings* m_settings;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(IPlugin::State)

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(IPlugin, "com.RTPTechGroup.Raindrop.IPlugin/1.0")
QT_END_NAMESPACE

#endif
