#ifndef EXTENDEDAPPLICATION_H
#define EXTENDEDAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QPointer>
#include <QSettings>
#include "pluginmanager.h"

class ExtendedApplication : public QApplication
{
    Q_OBJECT
public:
    explicit ExtendedApplication(int &argc, char **argv);
    ~ExtendedApplication();
    static ExtendedApplication *instance();

    void localize();
    bool initialize();

    //Settings
    void setSettings(QPointer<QSettings> settings);
    QPointer<QSettings> settings();
    void readSettings();
    void writeSettings();

signals:

public slots:

private:
    QPointer<QSettings> m_settings;
    QPointer<PluginManager> m_pluginManager;
};

#endif // EXTENDEDAPPLICATION_H
