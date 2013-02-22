#include <QtGui/QApplication>
#include <QMessageBox>
#include <QDebug>
#include "QtCasim.h"
#include "AutomataFactory.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtCasim casim;
	casim.show();

	return a.exec();
}
