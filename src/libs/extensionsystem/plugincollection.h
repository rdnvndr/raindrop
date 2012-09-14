#ifndef PLUGINCOLLECTION_H
#define PLUGINCOLLECTION_H

#include "iplugin.h"

#include <QObject>
#include <QString>
#include <QHash>
#include <QMessageBox>
#include <QPointer>

//! Класс коллекции плагинов
/*! Предназначен для управление колекцией плагинов
  */
class PluginCollection : public QObject
{
    Q_OBJECT
public:
    explicit PluginCollection(QObject *parent = 0);
    ~PluginCollection();

    //! Довбление плагина в коллекцю
    void addPlugin(IPlugin * plugin);

    //! Удаление плагина из коллекции
    void removePlugin(IPlugin * plugin);

    //! Получение плагина по имени
    IPlugin * pluginByName(const QString &name) const;

    //! Получение плагина по имени его класса
    IPlugin * pluginByClassName(const QString &className) const;

    QHash<QString, IPlugin *> plugins() const;

    //! Вызывает функцию initialize вовсех плагинах
    bool initializePlugins();

    //! Вызывает функцию release во всех плагинаха
    bool releasePlugins();

    //! Вызывает функцию delete во всех плагинах
    void deletePlugins();

private:
    bool initializePlugin(IPlugin * plugin);
    bool releasePlugin(IPlugin * plugin);

public slots:

signals:
    void showMessage(const QString &message, int alignment = Qt::AlignLeft | Qt::AlignBottom ,
                                            const QColor &color = Qt::black);
private:
    QHash<QString, IPlugin*> m_plugins;
};

#endif // PLUGINCOLLECTION_H
