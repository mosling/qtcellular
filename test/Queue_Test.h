#ifndef QUEUE_TEST_H
#define QUEUE_TEST_H

#include <QObject>
#include "Queue.h"

class Queue_Test : public QObject
{
    Q_OBJECT

public:
    Queue_Test();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testEnqueue();
    void testHead();
    void testDequeue();

private:
    Queue *q;
};

#endif // QUEUE_TEST_H
