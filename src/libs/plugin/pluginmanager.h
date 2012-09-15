#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QtGui>
#include "pluginglobal.h"
#include "iplugin.h"

class PLUGINLIB IPlugin;
typedef QHash < QString,IPlugin* > IPlugList;

//! Класс для управления плагинами
/*! Данный класс предназначен для загрузки плагинов,
    получения объектов плагина, сохранение настроек плагина.\n
    Пример использования:
    \code
        QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("Raindrop"));
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

    //! Получение объекта по имени
    /*! \param name Имя объекта
        Предназначено для получения объекта из вне из общего списка
        менеджера плагинов по его имени. Для приведения объекта к
        типу объекта плагина рекомендуется использовать static_cast.
        При компиляции при помощи VS необходимо в pro фаил добавить:
        QMAKE_LFLAGS += -FORCE:UNRESOLVED
        \sa addObject(),removeObject()
    */
    QObject *getObjectByName(const QString &name) const;

    //! Получение объекта по имени класса
    /*! \param className Имя класса
        Предназначено для получения объекта из вне из общего списка
        менеджера плагинов по его имени класса. Для приведения объекта к
        типу объекта плагина рекомендуется использовать static_cast.
        При компиляции при помощи VS необходимо в pro фаил добавить:
        QMAKE_LFLAGS += -FORCE:UNRESOLVED
        \sa addObject(),removeObject()
    */
    QObject *getObjectByClassName(const QString &className) const;

    //! Получение плагина по имени его класса
    IPlugin *getPlugin(const QString &className) const;

    //! Получение имени класса по плагину
    QString pluginClass(IPlugin* plug) const;

    //! Инициализация плагина
    /*! \param plug Плагин для инициализации.
        Производит настройку плагина после его загрузки
    */
    bool initPlugin(IPlugin* plug);

    //! Получить зависимые плагины
    /*! Позволяет получить список зависимых плагинов от указанного
     */
    QList<IPlugin*> dependentPlugins(IPlugin* plug);

    //! Добавления объекта в общий список менеджера плагинов
    /*! \param obj Объект для добавления в общий список менеджера плагинов
        При добавлении объекта в общий список менеджера плагинов
        этот объект можно получить из вне при помощи менеджера плагинов
        \sa getObjectByName()и getObjectByClassName()
    */
    void addObject(QObject *obj);

    //! Удаление объекта из общего списка менеджера плагинов
    /*! \param obj Объект для удаления из общего списка менеджера плагинов.
        В результате удаления объекта из общего списка менеджера плагинов
        он становится не доступен для доступа из вне при помощи менеджера плагинов.
        \sa getObjectByName()и getObjectByClassName()
    */
    void removeObject(QObject *obj);

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
    void showMessage(const QString &message, int alignment = Qt::AlignLeft | Qt::AlignBottom ,
                                            const QColor &color = Qt::black);

private slots:
    void removePlugin(QObject* obj);

private:
    //! Экземпляр менеджера плагинов
    static PluginManager *m_instance;

    //! Настройки
    QSettings *m_settings;

    //! Список объектов области видимости
    /*! Список объектов области видимости. Предназначен для
        получения их из вне
    */
    QHash<QString,QObject *> m_objects;

    //! Список плагинов
    /*! В списке плагинов хранятся загруженные плагины
    */
    IPlugList plugList;

};

#endif // PLUGINMANAGER_H
