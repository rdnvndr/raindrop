#include <QObject>
#include <QtTest/QtTest>

class  tst_%TestName% : public QObject
{
    Q_OBJECT

public:

    //! Конструктор теста
    explicit tst_%TestName%(QObject *parent = 0);

    //! Конструктор теста
    virtual ~tst_%TestName%();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
};

tst_%TestName%::tst_%TestName%(QObject *parent)
{

}

tst_%TestName%::~tst_%TestName%()
{

}

void tst_%TestName%::initTestCase()
{

}

void tst_%TestName%::cleanupTestCase()
{

}

void tst_%TestName%::init()
{

}

void tst_%TestName%::cleanup()
{

}
QTEST_MAIN(tst_%TestName%)
#include "tst_%TestName:l%.moc"
