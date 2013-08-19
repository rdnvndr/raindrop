#include <QObject>
#include <QtTest/QtTest>
#include <QSettings>
#include <plugin/pluginmanager.h>

class tst_IPlugin : public QObject
{
    Q_OBJECT

public:
    explicit tst_IPlugin(QObject *parent = 0);
    virtual ~tst_IPlugin();
    
public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

};

tst_IPlugin::tst_IPlugin(QObject *parent)
{
    Q_UNUSED(parent)
}

tst_IPlugin::~tst_IPlugin()
{

}

void tst_IPlugin::initTestCase()
{
    QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("tst_iplugin"));

    PluginManager pluginManager;
    pluginManager.setSettings(settings);
    pluginManager.loadPlugins();
}

void tst_IPlugin::cleanupTestCase()
{

}

void tst_IPlugin::init()
{

}

void tst_IPlugin::cleanup()
{

}
QTEST_MAIN(tst_IPlugin)
#include "tst_iplugin.moc"
