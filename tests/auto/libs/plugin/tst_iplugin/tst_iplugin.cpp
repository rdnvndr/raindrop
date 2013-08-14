#include <QObject>
#include <QtTest/QtTest>

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

}

tst_IPlugin::~tst_IPlugin()
{

}

void tst_IPlugin::initTestCase()
{

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
