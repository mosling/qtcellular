#include <QString>
#include <QtTest>

class CQueue_Test : public QObject
{
    Q_OBJECT
    
public:
    CQueue_Test();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

CQueue_Test::CQueue_Test()
{
}

void CQueue_Test::initTestCase()
{
}

void CQueue_Test::cleanupTestCase()
{
}

void CQueue_Test::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(CQueue_Test)

#include "cqueue_test.moc"
