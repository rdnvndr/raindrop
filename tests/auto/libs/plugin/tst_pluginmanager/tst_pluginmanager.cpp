#include <QObject>
#include <QtTest/QtTest>

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
};

tst_PluginManager::tst_PluginManager(QObject *parent)
{
    
}

tst_PluginManager::~tst_PluginManager()
{

}

void tst_PluginManager::initTestCase()
{
    
}

void tst_PluginManager::cleanupTestCase()
{
    
}

void tst_PluginManager::init()
{
    
}

void tst_PluginManager::cleanup()
{
    
}
QTEST_MAIN(tst_PluginManager)
#include "tst_pluginmanager.moc"

