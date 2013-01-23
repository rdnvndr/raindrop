#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include "pluginglobal.h"
#include "iplugin.h"

class PLUGINLIB IPlugin;

//! Класс для управления плагинами
/*! Данный класс предназначен для загрузки плагинов,
    получения объектов плагина, сохранение настроек плагина.\n
    Пример использования:
    \code
        QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("UniversalPlatform"));
        PluginManager pluginManager;
        pluginManager.setSettings(settings);
        pluginManager.loadPlugins();
    \endcode

*/
class PLUGINLIB PluginManager : public QObject
{
    Q_OBJECT
public:
    //! Получение экземпляра менеджера плагинов
    static PluginManager *instance();

    //! Конструктор плагина
    /*! Производит инициализацию плагина при его создании
    */
    PluginManager(QObject *parent = 0);

    //! Деструктор плагина
    /*! Производит завершающие действия при удалении плагина
    */
    ~PluginManager();

    //! Получение объекта для указанного интерфейса
    QObject *interfaceObject(QString interfaceName);

    //! Получение объектов для указанного интерфейса
    QList<QObject *> interfaceObjects(QString interfaceName);

    //! Инициализация плагина
    /*! \param plug Плагин для инициализации.
        Производит настройку плагина после его загрузки
    */
    bool initPlugin(IPlugin* plug);

    //! Получить зависимые плагины
    /*! Позволяет получить список зависимых плагинов от указанного
     */
    QList<IPlugin*> dependentPlugins(IPlugin* plugin);

    //! Получить от каких плагинов зависит
    /*! Позволяет получить список плагинов от которых зависит указанный плвгин
     */
    QList<IPlugin *> dependPlugins(IPlugin *plugin);

    //! Загрузка плагинов
    /*! Производит поиск плагина на диске в каталоге
        plugins запущенного приложения и его загрузку
    */
    void loadPlugins();

    //! Устанавливает ссылку на объект для сохранения настроек
    void setSettings(QSettings *s);

    //! Получает ссылку на объект для сохранения настроек
    QSettings *settings() const;

signals:
    //! Сигнал с сообщениями о загрузке плагинов
    void showMessage(const QString &message, int alignment = Qt::AlignLeft | Qt::AlignBottom ,
                                            const QColor &color = Qt::black);
private slots:
    //! Удаляет указанный плагин
    void removePlugin(QObject* obj);

private:
    //! Экземпляр менеджера плагинов
    static PluginManager *m_instance;

    //! Настройки
    QSettings *m_settings;

    //! Список плагинов
    /*! В списке плагинов хранятся загруженные плагины, которые можно получить
     *  по интерфейсам
     */
    QMultiHash<QString, QObject *> m_interfaces;
};

#endif // PLUGINMANAGER_H
