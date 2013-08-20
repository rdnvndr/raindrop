#include <QObject>
#include <QtTest/QtTest>
#include <QSettings>
#include <plugin/pluginmanager.h>
#include "pluginloadlog.h"

class  tst_PluginManager : public QObject
{
    Q_OBJECT
    
public:
    
    //! Конструктор теста
    explicit tst_PluginManager(QObject *parent = 0);
    
    //! Конструктор теста
    virtual ~tst_PluginManager();
    
public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void instance();
    void interfaceObject();
    void interfaceObjects();
    void dependentPlugins();
    void dependPlugins();
    void loadPlugins();
    void settings();

private:
    PluginManager m_pluginManager;
    PluginLoadLog *m_pluginLoadLog;
};

tst_PluginManager::tst_PluginManager(QObject *parent)
{
    Q_UNUSED(parent)
}

tst_PluginManager::~tst_PluginManager()
{

}

void tst_PluginManager::initTestCase()
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    // Подключение перевода для Qt
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(this);
    if (translator->load(translatorFileName,
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(translator);

    m_pluginLoadLog = new PluginLoadLog();
    connect(PluginManager::instance(),SIGNAL(loadedPlugin(QObject*)),
            m_pluginLoadLog, SLOT(loadPlugin(QObject*)));
    m_pluginManager.loadPlugins();
}

void tst_PluginManager::cleanupTestCase()
{
    PluginLoadLog *pluginUnloadLog = new PluginLoadLog();
    connect(PluginManager::instance(),SIGNAL(removedPlugin(QObject*)),
            pluginUnloadLog, SLOT(loadPlugin(QObject*)));
    delete m_pluginManager.interfaceObject("Itst_plugin3");
    QStringList plugins = pluginUnloadLog->loadedPlugins();
    QCOMPARE(plugins.count(), 4);
    if (plugins.at(0) == "tst_plugin2") {
        QVERIFY(plugins.at(0) == "tst_plugin2");
        QVERIFY(plugins.at(1) == "tst_plugin1");
        QVERIFY(plugins.at(2) == "tst_plugin4");
    } else {
        QVERIFY(plugins.at(0) == "tst_plugin1");
        QVERIFY(plugins.at(1) == "tst_plugin2"
                || plugins.at(1) == "tst_plugin4");
        QVERIFY(plugins.at(2) == "tst_plugin2"
                || plugins.at(2) == "tst_plugin4");
    }
    QVERIFY(plugins.at(3) == "tst_plugin3");
    delete pluginUnloadLog;

    delete m_pluginLoadLog;
}

void tst_PluginManager::init()
{
    
}

void tst_PluginManager::cleanup()
{
    
}

void tst_PluginManager::instance()
{
    QCOMPARE(&m_pluginManager, PluginManager::instance());
}

void tst_PluginManager::interfaceObject()
{
    QCOMPARE (m_pluginManager.interfaceObject("Itst_plugin1")->metaObject()->className(),
              "tst_plugin1");
    QCOMPARE (m_pluginManager.interfaceObject("Itst_plugin2")->metaObject()->className(),
              "tst_plugin2");
    QCOMPARE (m_pluginManager.interfaceObject("Itst_plugin3")->metaObject()->className(),
              "tst_plugin3");
}

void tst_PluginManager::interfaceObjects()
{
    QList<QObject *> listObject = m_pluginManager.interfaceObjects("Itst_plugin4");
    QVERIFY(listObject.count() == 2);

    QStringList objectName;
    objectName << "tst_plugin3" << "tst_plugin4";

    foreach (QObject *interfaceObject, listObject) {
        QVERIFY(objectName.removeOne(interfaceObject->metaObject()->className()));
    }
}

void tst_PluginManager::dependentPlugins()
{
    QStringList dependentName1;
    QStringList dependentName2;

    QStringList dependentName3;
    dependentName3 << "tst_plugin1" << "tst_plugin2" << "tst_plugin4";

    QStringList dependentName4;
    dependentName4 << "tst_plugin1";

    QList<QObject *> plugins = m_pluginManager.interfaceObjects("IPlugin");
    foreach (QObject *plugin, plugins) {
        IPlugin *iplugin = qobject_cast<IPlugin *>(plugin);
        QList<IPlugin *> listDependent = m_pluginManager.dependentPlugins(iplugin);
        foreach (IPlugin *pluginDependent, listDependent) {
            if (iplugin->name() == QString("tst_plugin1")) {
                QVERIFY(dependentName1.removeOne(pluginDependent->name()));
            } else if (iplugin->name() == QString("tst_plugin2")) {
                QVERIFY(dependentName2.removeOne(pluginDependent->name()));
            } else if (iplugin->name() == QString("tst_plugin3")) {
                QVERIFY(dependentName3.removeOne(pluginDependent->name()));
            } else if (iplugin->name() == QString("tst_plugin4")) {
                QVERIFY(dependentName4.removeOne(pluginDependent->name()));
            }
        }
    }
    QCOMPARE(dependentName1.count(),0);
    QCOMPARE(dependentName2.count(),0);
    QCOMPARE(dependentName3.count(),0);
    QCOMPARE(dependentName4.count(),0);
}

void tst_PluginManager::dependPlugins()
{
    QStringList dependName1;
    dependName1 << "tst_plugin3" << "tst_plugin4";

    QStringList dependName2;
    dependName2 << "tst_plugin3";

    QStringList dependName3;

    QStringList dependName4;
    dependName4 << "tst_plugin3";

    QList<QObject *> plugins = m_pluginManager.interfaceObjects("IPlugin");
    foreach (QObject *plugin, plugins) {
        IPlugin *iplugin = qobject_cast<IPlugin *>(plugin);
        QList<IPlugin *> listDepend = m_pluginManager.dependPlugins(iplugin);
        foreach (IPlugin *pluginDepend, listDepend) {
            if (iplugin->name() == QString("tst_plugin1")) {
                QVERIFY(dependName1.removeOne(pluginDepend->name()));
            } else if (iplugin->name() == QString("tst_plugin2")) {
                QVERIFY(dependName2.removeOne(pluginDepend->name()));
            } else if (iplugin->name() == QString("tst_plugin3")) {
                QVERIFY(dependName3.removeOne(pluginDepend->name()));
            } else if (iplugin->name() == QString("tst_plugin4")) {
                QVERIFY(dependName4.removeOne(pluginDepend->name()));
            }
        }
    }
    QCOMPARE(dependName1.count(),0);
    QCOMPARE(dependName2.count(),0);
    QCOMPARE(dependName3.count(),0);
    QCOMPARE(dependName4.count(),0);
}

void tst_PluginManager::loadPlugins()
{
    QStringList plugins = m_pluginLoadLog->loadedPlugins();
    QCOMPARE(plugins.count(), 4);

    QVERIFY(plugins.at(0) == "tst_plugin3");
    QVERIFY(plugins.at(1) == "tst_plugin2");
    QVERIFY(plugins.at(2) == "tst_plugin1"
            || plugins.at(2) == "tst_plugin4");
    QVERIFY(plugins.at(3) == "tst_plugin1"
            || plugins.at(3) == "tst_plugin4");
}

void tst_PluginManager::settings()
{
    QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("Raindrop"));
    m_pluginManager.setSettings(settings);

    QCOMPARE(settings, m_pluginManager.settings());
}
QTEST_MAIN(tst_PluginManager)
#include "tst_pluginmanager.moc"

