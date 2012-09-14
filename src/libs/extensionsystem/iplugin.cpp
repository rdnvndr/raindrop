#include "iplugin.h"
#include "pluginmanager.h"

/*! Этот класс предназначен для создания пользовательского
   плагина путем наследования данного класса. Работа с
   плагинами производится при помощи класса PluginManager
   \n
   Пример: \n
   \b exampleplug.pro
   \code
        LIBS += -lextensionsystem -ltreedockwidget -lmdiextarea -lmainwindow
        include(../plug.pri)

        TARGET =
        DEPENDPATH += .
        INCLUDEPATH += .

        # Input
        HEADERS += exampleplug.h
        SOURCES += exampleplug.cpp
        RESOURCES += qrc.qrc
   \endcode
   \b exampleplug.h
   \code
        #ifndef PLUG_EXAMPLE_H
        #define PLUG_EXAMPLE_H

        #include <iguiplugin.h>
        #include <QtGui>

        class ExamplePlug: public IGuiPlugin
        {
            Q_OBJECT
            Q_INTERFACES(IGuiPlugin)

        public:
            ExamplePlug(QObject *parent = 0);

            bool initialize();
            bool release();
            void readSettings();
            void writeSettings();

        public slots:
            void createTab();
            void createDlgWindow();
        };

        #endif
   \endcode
   \b exampleplug.cpp
   \code
        #include "exampleplug.h"
        #include <treedockwidget/treedockwidget.h>
        #include <mdiextarea/mdiextarea.h>
        #include <mainwindow/mainwindow.h>
        #include <pluginmanager.h>

        ExamplePlug::ExamplePlug(QObject *parent):
            IGuiPlugin(parent)
        {
            setName("ExamplePlug");
            setIcon(QIcon(":icon/icons/example.png"));
            setDescript(tr("Пример плагина"));
            setVendor(tr("RTPTechGroup"));
            setVersion("0.0.1");
            addDepend("TreeCommands");
        }

        bool ExamplePlug::initialize()
        {
            PluginManager* pluginManager = PluginManager::instance();
            TreeDockWidget* dockWidget = qobject_cast<TreeDockWidget*>(pluginManager->getObjectByName(
                                                       "TreeCommandsPlug::TreeDockWidget"));

            QTreeWidget* tree = dockWidget->insertTreeWidget(QIcon(":/example"),tr("Проверка"));

            QList<QTreeWidgetItem*> items;
            // Создаем узел - раздел в дереве
            QTreeWidgetItem* item = new QTreeWidgetItem();
            // Создаем шрифт
            QFont font = item->font(0);
            font.setBold(true);
            // Устанавливаем шрифт
            item->setFont(0, font);
            // Устанавливаем имя узлу - разделу
            item->setText(0, tr("Примеры"));
            items.append(item);

            // Создаем узел для создания закладки
            QTreeWidgetItem* itemnode = new QTreeWidgetItem(item);
            itemnode->setIcon(0, QIcon(":/example"));
            itemnode->setText(0, tr("Создание закладки"));
            dockWidget->setFuncTreeItem(itemnode, this,"createTab");

            // Создаем узел для создания диалогового окна
            itemnode = new QTreeWidgetItem(item);
            itemnode->setIcon(0, QIcon(":/example"));
            itemnode->setText(0, tr("Создание диалога"));
            dockWidget->setFuncTreeItem(itemnode, this,"createDlgWindow");
            tree->insertTopLevelItems(0,items);

            return true;
        }

        bool ExamplePlug::release()
        {
            return true;
        }

        void ExamplePlug::readSettings()
        {
        }

        void ExamplePlug::writeSettings()
        {
        }

        void ExamplePlug::createTab()
        {
            PluginManager* pluginManager = PluginManager::instance();
            MainWindow* mainwindow = qobject_cast<MainWindow*>(pluginManager->getObjectByName(
                                                       "MainWindowPlug::MainWindow"));
            QMdiSubWindow* subWindow =  mainwindow->addSubWindow(new QTextEdit(mainwindow));
            if (subWindow!=NULL)
                subWindow->setWindowTitle(tr("Пример редактора"));
        }

        void ExamplePlug::createDlgWindow()
        {
            QMessageBox::warning(NULL,tr("Инициализация"), tr("Инициализация плагина Example"));
        }

        Q_EXPORT_PLUGIN2(example, ExamplePlug)


   \endcode
*/

IPlugin::IPlugin(QObject *parent) :
    QObject(parent)
{
    state = IPlugin::NoInit;
    setCategory(tr("Основные"));
}

IPlugin::~IPlugin()
{

}

bool IPlugin::release()
{
    return true;
}

bool IPlugin::initialize()
{
    return true;
}

void IPlugin::readSettings()
{
}

void IPlugin::writeSettings()
{
}

/*!
 *  \brief Имя плагина
 *
 *  Имя плагина уникально идентифицирует плагин в среде выполнения.
 *  Обычно имя плагина совпадает с именем модуля содержащего его.
 *  \sa setName(), addDependency()
 */
QString IPlugin::name() const
{
    return m_name;
}

QIcon IPlugin::icon()
{
    return m_Icon;
}

/*!
 * \brief Описание плагина
 * \return
 */
QString IPlugin::descript() const
{
    return m_descript;
}

/*!
 *  \brif Категория плагина
 *
 *  Категория плагинов предназначена для группировки плагинов по смыслу в одну группу.
 */
QString IPlugin::category() const
{
    return m_category;
}

/*!
 *  \brief Версия плагина
 */
QString IPlugin::version() const
{
    return m_version;
}

/*!
 *  \brief Производитель плагина
 */
QString IPlugin::vendor() const
{
    return m_vendor;
}

/*!
 * \brief Установка имени плагина
 * \param name
 */
void IPlugin::setName(const QString name)
{
    m_name = name;
}

void IPlugin::setIcon(QIcon icon)
{
   m_Icon = icon;
}

/*!
 * \brief IPlugin::setDescript
 * \param descript
 */
void IPlugin::setDescript(QString descript)
{
    m_descript = descript;
}

/*!
 *  \brief Установка категории плагина
 */
void IPlugin::setCategory(const QString category)
{
    m_category = category;
}

/*!
 *  \brief Установка версии плагина
 */
void IPlugin::setVersion(const QString version)
{
    m_version = version;
}

/*!
 *  \brief Установка производителя плагина
 */
void IPlugin::setVendor(const QString vendor)
{
    m_vendor = vendor;
}

/*!
 *  \brief Проверка на состояние ошибки
 */
bool IPlugin::hasError() const
{
    return m_hasError;
}

/*!
 *  \brif Установка состояни ошибки
 */
void IPlugin::setError()
{
    m_hasError = true;
}

/*!
 *  \brief Сброс состояния ошиибки
 */
void IPlugin::resetError()
{
    m_hasError = false;
}

/*!
 *  \brief Сообщение ошибки
 */
QString IPlugin::errorString() const
{
    return m_errorString;
}

/*!
 *  \brif Установка сообщения ошибки
 */
void IPlugin::setErrorString(const QString error)
{
    m_errorString = error;
}

/*!
 *  \brief Настроки плагина
 */
QPointer<QSettings> IPlugin::settings()
{
    return m_settings;
}

/*!
 *  \brief Устанавливает экземпляр класса настроек
 */
void IPlugin::setSettings(QPointer<QSettings> settings)
{
    m_settings = settings;
}

/*! \brief Список зависимостей плагина
 *
 *  Список зависимостей содержт имена плагинов от которых он имеет зависимости
 */
QStringList IPlugin::dependence()
{
    return m_dependence;
}

/*!
 * \brief Список зависимых плагинов
 * \return
 */
QStringList IPlugin::clients()
{
    return m_clients;
}

/*!
 *  \brief Добавление зависимости от других плагинов
 *  \param pluginName имя плагина от которогимеется зависимость
 *
 *  При вызове данного метода добавляются зависимости
    от других плагинов. При инициализации плагинов определяются
    зависимости и в случае если зависимый плагин не инициализирован
    инициализируется сначало зависимый плагин. Имя указанное в параметре
    должно совпадать с именм плагина.
    \sa name(); setName(), PluginCollection::initializePlugins()
*/
void IPlugin::addDepend(const QString pluginName)
{
    m_dependence.append(pluginName);
}

/*!
 * \brief Добавление плагина клиента
 * \param pluginName
 */
void IPlugin::addClient(const QString pluginName)
{
    m_clients.append(pluginName);
}

/*!
 * \brief Публикация объекта.
 * \param obj Объект подлежащий добавлению в колекцию публичного доступа
 *
 * При вызове данного метода в колекцию объектов менеджера плагинов добавляется
 * переданный параметром объект
 */
void IPlugin::publicObject(QPointer<QObject> obj)
{
    obj->setObjectName(metaObject()->className()+QString("::")+obj->objectName());
    PluginManager::instance()->addObject(obj);
}

/*! Сокрытие ранее пудличного объекта
    \param obj Объект подлежащий удалению из колекции публичного доступа
    При вызове данного метода из колекции объектов менеджера плагинов удаляется переданный параметром объект
  */
void IPlugin::privateObject(QPointer<QObject> obj)
{
    QString objName = obj->objectName();
    objName = objName.left(objName.indexOf("::")+3);
    obj->setObjectName(objName);
    PluginManager::instance()->removeObject(obj);
}


