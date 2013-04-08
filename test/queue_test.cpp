#include <QString>
#include <QtTest>
#include <QDebug>
#include "Queue_Test.h"


Queue_Test::Queue_Test()
{
}

void Queue_Test::initTestCase()
{
    q = new Queue();
}

void Queue_Test::cleanupTestCase()
{
    delete q;
}

void Queue_Test::testEnqueue()
{
  q->clear();
  QVERIFY2(q->isempty()==true, "not empty after clear");
  q->enqueue(1,1);
  QVERIFY2(q->isempty()==false, "empty after enqueue a element");
  q->enqueue(2,2);
  q->enqueue(3,3);
  q->enqueue(4,4);
}

void Queue_Test::testHead()
{
    cell *c;
    c = q->getHead(); QVERIFY(c->type==1 && c->value==1);
    c = q->getHead(); QVERIFY(c->type==1 && c->value==1);
    q->dequeue();
    c = q->getHead(); QVERIFY(c->type==2 && c->value==2);
    q->reverse();
    c = q->getHead(); QVERIFY(c->type==4 && c->value==4);
    q->dequeue();
    c = q->getHead(); QVERIFY(c->type==3 && c->value==3);
}

void Queue_Test::testDequeue()
{

}
