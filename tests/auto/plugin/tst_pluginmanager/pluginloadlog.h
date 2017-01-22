#ifndef PLUGINLOADLOG_H
#define PLUGINLOADLOG_H

#include <QObject>
#include <plugin/iplugin.h>

class PluginLoadLog : public QObject
{
    Q_OBJECT
public:
    explicit PluginLoadLog(QObject *parent = 0);
    virtual ~PluginLoadLog();

    QStringList loadedPlugins();
    void clearLoadedPlugins();

public slots:
    void loadPlugin(QObject *plugin);

private:
    QStringList m_loadedPlugins;
    
};

#endif // PLUGINLOADLOG_H
