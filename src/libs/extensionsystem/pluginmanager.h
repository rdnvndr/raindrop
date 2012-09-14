#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "extensionsystemglobal.h"
#include "pluginmodel.h"
#include "iplugin.h"
#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QWidget>
#include <QPointer>

class PluginCollection;
class ObjectCollection;
class PluginViewDlg;

//! Класс для управления плагинами
/*! Данный класс предназначен для загрузки плагинов,
    получения объектов плагина, сохранение настроек плагина.\n
    Пример использования:
    \code
        QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("UniversalPlatform"));

        PluginManager *pluginManager = PluginManager::instance();
        pluginManager->setSettings(settings);
        pluginManager->loadPlugins();

        ...

        delete pluginManager;
    \endcode

*/
class EXTENSIONSYSTEMSHARED_EXPORT PluginManager : public QObject
{
    Q_OBJECT
protected:
    explicit PluginManager(QObject *parent = 0);

public:
    virtual ~PluginManager();

    //! Предоставляет экземпляр менеджера плагинов
    static QPointer<PluginManager> instance();

    static void release();

    //! Установка глобалных настроек
    void setSettings(QPointer<QSettings> settings);

    //! Настройки
    QPointer<QSettings> settings();

    //! Запись настроек
    void writeSettings();

    //! Чтение настроек
    void readSettings();

    //! Добавление объекта в колекцию менеджера плагинов
    void addObject(QPointer<QObject> obj);

    //! Удаление объекта из колекции менеджера плагинов
    void removeObject(QPointer<QObject> obj);

    //! Предоставляет объект по его имени
    QPointer<QObject> objectByName(const QString &name) const;

    //! Предоставляет объект по имени его класса
    QPointer<QObject> objectByClassName(const QString &className) const;

    //! Загрузка плагинов в колекцию
    bool loadPlugins();

    //! Выгрузка плагинов
    void unloadPlugins();

    //! Предоставляет список путей поиска плагинов
    QStringList pluginPaths() const;

    //! Установка путей поиска плагинов
    void setPluginPaths(const QStringList &paths);

    //! Получение плагина по имени
    IPlugin * pluginByName(const QString &name) const;

    //! Получение плагина по имени его класса
    IPlugin * pluginByClassName(const QString &className) const;

    //! Отображение информации о загруженных плагинах
    void showPluginView(QWidget *parent);

private:
    QPointer<PluginCollection> pluginCollection();
    QPointer<ObjectCollection> objectCollection();

signals:
    void showMessage(const QString &message, int alignment = Qt::AlignLeft | Qt::AlignBottom ,
                                            const QColor &color = Qt::black);

public slots:

private:
    //! Экземпляр класса менеджера плагинов
    static QPointer<PluginManager> m_instance;

    //! Настройки
    QPointer<QSettings> m_settings;

    //! Список путей к файлам плагинов
    QStringList m_pluginPaths;

    //! Коллекция плагинов
    /*! В коллекция плагинов хранятся загруженные плагины
    */
    QPointer<PluginCollection> m_pluginCollection;

    //! Коллекция объектов области видимости
    /*! Коллекция объектов области видимости.
         Предоставляет доступ к объектам одних плагинов другим в любой точке программы.
    */
    QPointer<ObjectCollection> m_objectCollection;

    //! Окно отображающее информацию о загруженных плагинах
    QPointer<PluginViewDlg> m_pluginViewDlg;
    PluginModel m_pluginModel;
};

#endif // PLUGINMANAGER_H
