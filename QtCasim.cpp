#include <QDebug>
#include "QtCasim.h"
#include "StopWatch.h"
#include "FieldAlgorithms.h"
#include <QFileDialog>
#include "ParamDialog.h"

QtCasim::QtCasim(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), fieldWidgetLayout(NULL), fieldWidget(NULL),
  automata(NULL)
{
	ui.setupUi(this);
}

QtCasim::~QtCasim()
{
	delete fieldWidgetLayout; fieldWidgetLayout = NULL;
}

void QtCasim::setVisualizationWidget(QWidget *aVis)
{
	if (fieldWidgetLayout == NULL)
	{
		fieldWidgetLayout = new QVBoxLayout(ui.paintArea);
	}
	else
	{
		// altes Feld wird nicht mehr ben�tigt
		fieldWidgetLayout->removeWidget(fieldWidget);
		delete fieldWidget;
	}

	fieldWidgetLayout->addWidget(aVis);
	fieldWidget = aVis;
	fieldWidget->repaint();
}

void QtCasim::setAutomata(Automata *aAutomata)
{
	automata = aAutomata;
	if (fieldWidget!=NULL) fieldWidget->update();
}

void QtCasim::on_actionClear_triggered()
{
	 automata->getField()->clearField();
	 if (fieldWidget!=NULL) fieldWidget->update();
}

void QtCasim::on_actionRandom_triggered()
{
	 FieldAlgorithms::Randomize(automata->getField(), automata->getStateList());
	 if (fieldWidget!=NULL) fieldWidget->update();
}

void QtCasim::on_actionSaveField_triggered()
{
	 QString srcpath = QFileDialog::getSaveFileName(this, "Datei auswaehlen");
	 automata->getField()->saveField(srcpath);
}

void QtCasim::on_actionLoadField_triggered()
{
	QString srcpath = QFileDialog::getOpenFileName(this, "Datei auswaehlen");
	automata->getField()->loadField(srcpath);
	if (fieldWidget!=NULL) fieldWidget->update();
}

void QtCasim::on_btnNext_clicked()
{
	StopWatch sw;

	sw.start();
	automata->nextConfiguration();
	sw.stop();
	qDebug() << "time to compute one configuration " << sw.getTimeString();
	qDebug() << "stable configuration " << automata->isStable();
	if (fieldWidget!=NULL) fieldWidget->update();
}

void QtCasim::on_btnStart_clicked()
{
	bStopped = false;
	StopWatch sw;
	unsigned int cnt = 0;

	sw.start();
	while (!bStopped)
	{
		automata->nextConfiguration();
		if (fieldWidget!=NULL) fieldWidget->repaint();
		qApp->processEvents();
		++cnt;
		if (automata->isStable())
		{
			qDebug() << "reach stable state";
			bStopped = true;
		}
	}
	sw.stop();
	qDebug() << "time to compute " << cnt
				<< " configuration " << sw.getTimeString()
				<< " ( " << cnt/sw.getTime() << "configuration/second )";
}

void QtCasim::on_btnStopp_clicked()
{
	qDebug() << "____________________________STOPP____________________";
	bStopped = true;
}

extern int parseAutomataFile(QString, AutomataFactory *);

void QtCasim::on_actionLoad_triggered()
{
	QString srcpath = QFileDialog::getOpenFileName(this, "Automat auswaehlen");

	qDebug() << "loading automata " << srcpath;

	delete automata;
	factory.init();

	if (parseAutomataFile(srcpath, &factory))
	{
		this->setVisualizationWidget(factory.getFieldWidget());
		this->setAutomata(factory.getAutomata());
	}
}


void QtCasim::on_actionTuring_triggered()
{
	ParamDialog *p = new ParamDialog(this);
	p->addInputField("XPosition","","");
	p->addInputField("YPosition","","");
	p->addInputField("Turing-State","","");
	p->addInputField("Direction","","");
	p->exec();

	automata->setParameter("XPOS", p->getFieldValue(0).toInt());
	automata->setParameter("YPOS", p->getFieldValue(1).toInt());
	automata->setParameter("STATE", p->getFieldValue(2).toInt());
	automata->setParameter("DIR", p->getFieldValue(3).toInt());

	automata->finishInit();
}

void QtCasim::on_actionMouse_State_triggered()
{
   ParamDialog *p = new ParamDialog(this);
   p->addInputField("State","","");
   p->exec();

   automata->getStateList()->setMouseState(p->getFieldValue(0).toInt());
}