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

	QtCasim casim;
	casim.show();

    return app.exec();
}
