#include <QApplication>
#include <QTranslator>
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>
#include "QtCasim.h"
#include "AutomataFactory.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load("qtcellular_en", ":/translations"))
    {
        app.installTranslator(&translator);
    }

//    Queue q;
//    q.parseExpression("CELL=alife AND (SUM(alife) = 2 OR SUM(alife) = 3)");

    QtCasim casim;
    casim.show();
    return app.exec();

    return 0;
}
