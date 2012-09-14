#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "extensionsystemglobal.h"
#include <QObject>
#include <QtPlugin>
#include <QFlags>
#include <QSettings>
#include <QStringList>
#include <QPointer>
#include <QIcon>

class EXTENSIONSYSTEMSHARED_EXPORT IPlugin : public QObject
{
    Q_OBJECT

public:
    /*! \brief Перечисление состояний плагина
     *
     * Описывает в каком состоянии находится плагин на данный момент
     * используется при инициализации плагина
     */
    enum StateFlags
    {
        //!< Плагин не инициализирован
        NoInit,
        //!< Плагин инициализирован
        Init,
        //!< Плагин заблокирован
        Lock,
        //!< Плагин с ошибкой
        Error
    };  

    //! Состояние плагина
    StateFlags state;

    IPlugin(QObject *parent = 0);
    virtual ~IPlugin();

    //! Инициализация плагина
    virtual bool initialize() = 0;

    //! Освобождение плагина
    virtual bool release() = 0;

    //! Чтение настроек
    virtual void readSettings();

    //! Запись настроек
    virtual void writeSettings();

    //! Имя плагина
    QString name() const;

    //! Иконка плагина
    QIcon icon();

    //! Описание плагина
    QString descript() const;

    //! Категория в которой состоит плагин
    QString category() const;

     //! Версия плагина
    QString version() const;

    //! Производитель плагина
    QString vendor() const;

    //! Установка имени плагина
    void setName(const QString name);
    //! Установка иконки плагина
    void setIcon(QIcon icon);
    //! Установка описания плагина
    void setDescript(QString descript);
    //! Установить категорию плагина
    void setCategory(const QString category);
    //! Установить версию плагина
    void setVersion(const QString version);
    //! Установить производителя плагина
    void setVendor(const QString vendor);
    //! Проверка на состояние ошибки
    bool hasError() const;
    //! Установка состояни ошибки
    void setError();
    //! Сброс состояния ошиибки
    void resetError();
    //! Вернуть сообщение ошибки
    QString errorString() const;
    //! Установка сообщения ошибки
    void setErrorString(const QString error);
    //! Настроки плагина
    QPointer<QSettings> settings();
    //! Устанавливает экземпляр класса настроек
    void setSettings(QPointer<QSettings> settings);
    //! Список зависимостей плагина
    QStringList dependence();
    //! Добавление зависимости от других плагинов
    void addDepend(const QString pluginName);
    //! Публикация объекта
    QStringList clients();
    void addClient(const QString pluginName);

    void publicObject(QPointer<QObject> obj);
    //! Сокрытие ранее пудличного объекта
    void privateObject(QPointer<QObject> obj);
signals:

public slots:

private:
    //Информация о модуле
    //! Имя плагина
    QString m_name;

    //! Описание плагина
    QString m_descript;

    //! Категория в которой состоит плагин
    QString m_category;

    //! Версия плагина
    QString m_version;

    //! Производитель плагина
    QString m_vendor;

    //! Флаг возникновения ошибки
    bool m_hasError;

    //! Сообщение ошибки
    QString m_errorString;

    //! Список плагинов от которых зависит плагин
    QStringList m_dependence;

    //! Список плагинов которые зависят от плагин
    QStringList m_clients;

    //! Экземпляр глобальных настроек
    QPointer<QSettings> m_settings;

    //! Иконка плагина
    QIcon m_Icon;
};

Q_DECLARE_INTERFACE(IPlugin, "com.RTPTechGroup.UniversalPlatform.IPlugin/1.0")
#endif // IPLUGIN_H
