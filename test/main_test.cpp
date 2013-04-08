#include <QTest>
#include "Queue_Test.h"

int main (int argc, char *argv[])
{
    Queue_Test qt;
    QTest::qExec (&qt, argc, argv);

    return 0;
}
