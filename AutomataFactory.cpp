#include <QtDebug>
#include "AutomataFactory.h"
#include "Expression.h"
#include "GridWidget.h"
#include "FieldAlgorithms.h"

AutomataFactory::AutomataFactory() :
automata(NULL),
automataStates(NULL),
mRules(NULL),
mCurrentRule(NULL),
automataField(NULL),
gridField(NULL),
automataFieldWidget(NULL),
turing(NULL)
{
}

AutomataFactory::~AutomataFactory()
{
}

void AutomataFactory::init()
{
	automataStates = new StateList();
	mRules = new RuleSet();
}

void AutomataFactory::setAutomataName (QString aName, etAutomataType aType)
{
	switch (aType)
	{
	case TURINGMODEL:
		qDebug() << "create turing model";
		turing = new Turing();
		automata = turing;
		automata->setParameter("NEWCELLMETHOD", 0);
		break;
	case CAMODEL:
		qDebug() << "create CA model";
		automata = new Ca();
		automata->setRuleSet(mRules);
		automata->setParameter("NEWCELLMETHOD", 0);
		break;
	case CA1DMODEL:
		qDebug() << "create CA1d model";
		//model = new Ca1d();
		break;
	case AGENTMODEL:
		qDebug() << "create agent model";
		//model = SelectAgent (name);
		break;
	default:
		qDebug() << "no exiting model found";
	}
}

void AutomataFactory::setSteps (int s1,int s2)
{
    qDebug() << "STEPS are deprecated with the visualization frontend";
}


//! Diese Methode wird nur aufgerufen, wenn es sich um ein Gitter handelt.
//! Dabei werden eine Reihe von Aktionen durchgeführt:
//! * Anlegen des Feldobjektes
//! * Anlegen des FeldVisualisierungsobjektes
void AutomataFactory::setArray (int x,int y)
{
	qDebug() << "create a grid array " << x << "," << y;
	gridField = new Grid();
	gridField->InitField(x, y);
	GridWidget *gw = new GridWidget(NULL);
	gw->setStateList(automataStates);
	gw->setGridData(gridField);
	automataField = gridField;
	automataFieldWidget = gw;
}

/*!
Diese Methode wird nur bei einer planaren Map aufgerufen.
*/
void AutomataFactory::addPlanarmap (char *fname)
{
	qDebug() << "create planar map from file " << fname;
	//model->newField(1);
	//field->InitField (fname);
}

void AutomataFactory::setBorderType(int bt, int ptb)
{
	qDebug() << "set border type " << bt << "; " << ptb;
	if (gridField) gridField->setBorder(bt, ptb);
}

void AutomataFactory::setParameter(QString aName, int aValue)
{
	qDebug() << "set parameter " << aName << "=" << aValue;
	if ("ADDCELL" == aName)
	{
		automataField->setAddingCell(aValue==1);
	}
}

int AutomataFactory::setVariable(QString aName, int aValue)
{
	qDebug() << "set variable " << aName << "=" << aValue;
	int vIdx = mRules->setVariable(aName, aValue);
	return vIdx;
}

/*!
 \brief
 Mit dieser Funktion wird der Name eines Zustandes oder der Werte
 einer definierten Variablen in die Datenstruktur übernommen.

 \param aName der gefundene Name
 \param aQueue Zeiger auf die Queue in die eingefügt werden soll
*/
void AutomataFactory::addStateOrVariable (QString aName, Queue *aQueue)
{
	int i = automataStates->getIndex(aName);

	if ( i != -1)
	{
		qDebug() << "found state " << aName;
		aQueue->enqueue(2,i);
	}
	else
	{
		i = mRules->getVarIndex(aName);
		if (-1 == i)
		{
			i = mRules->setVariable(aName, 0);
		}
		aQueue->enqueue (1,i);
	}
}


//! Hinzufügen einer Funktion bestimmten Typs. Mit dem Funktions-Typ
//! wird eine neue Regel gestartet.
//!
//! \param aFuncType Art der Funktion, Berechnung oder Bedingung
//! \param aQueue Die Datenstruktur, welche die Funktionszeichen enthält
void AutomataFactory::setFunction(etRulePart aFuncType, Queue *aQueue)
{
    QString vStr[5] = {"Result","Function","Condition","Moveto-Func1","Moveto-Func2"};
	Expression *exp = new Expression;

	qDebug() << "set function type " << vStr[(int)aFuncType];
	// aQueue->show();
	aQueue->reverse();
	exp->importQueue(aQueue);
	exp->show();
	if (RP_ACTION == aFuncType)
	{
		mCurrentRule = new Rule();
		mRules->addRule(mCurrentRule);
	}
	mCurrentRule->addExpression(exp, aFuncType);

	aQueue->clear();
}

void AutomataFactory::addState(int what,QString name, int num,
char c, QString s,int l, QString s2)
{
	automataStates->addState(what,name,num,c,s,l,s2);
}

void AutomataFactory::add_gridcolor (char *name)
{
}

void AutomataFactory::create_states (int what,int n,char *name,int c,int l)
{
	QString nname;
	int i;

	for (i=1; i<=n; i++)
	{
		nname = QString("%1_%2").arg(name).arg(i);
		switch (what)
		{
		case 0:
			addState (0,nname,c+i,'-',"",l,"");
			break;
		case 1:
			addState (3,nname,0,'-',"",l,"");
			break;
		}
	}
}

//! Hinzufügen einer von Neumann Nachbarschaft
//!
void AutomataFactory::addNeumannNeighborhood(int n)
{
	int i, j;

	qDebug() << "add neumann neighbourhood";
	for (i = -n; i <= n; i++)
	{
		for (j = -n; j <= n; j++)
		{
			if (abs(i) + abs(j) == n)
			{
				automataField->addNeighbor (j,i,0);
			}
		}
	}
}

void AutomataFactory::addMooreNeighborhood(int n)
{
	int i, j;

	qDebug() << "add moore neighbourhood";
	for (i = -n; i <= n; i++) {
		for (j = -n; j <= n; j++) {
			if ((abs(i) == n) || (abs(j) == n)) {
				automataField->addNeighbor (j,i,0);
			}
		}
	}
}

void AutomataFactory::addTuringFour ()
{
    QPoint mp[4]={QPoint(0,-1),QPoint(1,0),
                  QPoint(0,1),QPoint(-1,0)};
	int i;

	for (i=0;i<=3;i++)
	{
        automataField->addNeighbor (mp[i].x(), mp[i].y(), 0);
	}
}

void AutomataFactory::addTuringEight ()
{
    QPoint mp[8]={QPoint(0,-1),QPoint(1,-1),
                  QPoint(1,0),QPoint(1,1),
                  QPoint(0,1),QPoint(-1,1),
                  QPoint(-1,0),QPoint(-1,-1) };
	int i;

	for (i=0;i<=7;i++)
	{
        automataField->addNeighbor (mp[i].x(), mp[i].y(), 0);
	}
}

void AutomataFactory::addRadiusNeighborhood (int n)
{
	int i;

	qDebug() << "add radius";
	for (i = -n; i <= n; i++)
	{
		automataField->addNeighbor (i, -1, 0);
	}
}

//! Die interne (0,0) befindet sich oben links, das Feld ist
//! im Quadranten 4. Der Nutzer denk im Quadrant 1, deshalb
//! wird der y-Wert negiert.
void AutomataFactory::addPoint(int x,int y)
{
	qDebug() << "add neighbour point";
	automataField->addNeighbor (x, (-1)*y, 0);
}

//! Margolus ist eine 2x2 Block-Umgebung deren Startpunkt
//! alternierend wechselt.
void AutomataFactory::addMargolusNeighborhood ()
{	
	qDebug() << "add Margolus neighbourhood";
	automataField->addNeighbor( 1,  0, 0);
	automataField->addNeighbor( 1,  1, 0);
	automataField->addNeighbor( 0,  1, 0);

	automataField->addNeighbor( 0,  1, 1);
	automataField->addNeighbor(-1,  1, 1);
	automataField->addNeighbor(-1,  0, 1);

	automataField->addNeighbor(-1,  0, 2);
	automataField->addNeighbor(-1, -1, 2);
	automataField->addNeighbor( 0, -1, 2);

	automataField->addNeighbor( 0, -1, 3);
	automataField->addNeighbor( 1, -1, 3);
	automataField->addNeighbor( 1,  0, 3);

	automataField->setAddingCell(true);
	automata->setParameter("NEWCELLMETHOD", 3);
}

void AutomataFactory::setFieldSquare (int x,int y,int w,int c)
{
	int i,j;

	qDebug() << "set square";
	for (i=x;i<x+w;i++)
	{
		for (j=y;j<y+w;j++)
		{
            automataField->SetCellState(i-1,j-1,c);
		}
	}
}

void AutomataFactory::loadField (QString aFileName, bool isAscii)
{
	aFileName.replace("\"","");
	qDebug() << "load field " << aFileName << " ascii: " << isAscii;
	if (isAscii)
	{
		automataField->loadAsciiField(aFileName);
	}
	else
	{
		automataField->loadField(aFileName);
	}
}

void AutomataFactory::randomField ()
{	
	qDebug() << "initialize random field";
	if (automataField != NULL)
	{
		FieldAlgorithms::Randomize(automataField, automataStates);
	}
}


void AutomataFactory::set_observer (int which)
{
	qDebug() << "set observer " << which;
	/*
#ifdef DEBUG_MODE
 printf ("Debug:: Set Observer %d\n",which);
#endif
 switch (which) {
  case 1:
   sprintf (statistic_fn,"%s.states",automata_name);
   statistic_file = fopen(statistic_fn, "w");
   if (statistic_file == 0) {
 printf (SEMA_006,statistic_fn);
 *statistic_fn = '\0';
   } else {
 observe_states = TRUE;
 printf (SEMA_007,statistic_fn);
   }
   break;
  case 2:
   sprintf (fields_fn,"%s.fields",automata_name);
   fields_file = fopen(fields_fn, "w");
   if (fields_file == 0) {
 printf (SEMA_006, fields_fn);
 *fields_fn = '\0';
   } else {
 observe_fields = TRUE;
 printf (SEMA_008, fields_fn);
 fprintf (fields_file,"%d@%d@",max_xcells,max_ycells);
   }
   break;
  case 3:
   sprintf (changes_fn,"%s.changes",automata_name);
   changes_file = fopen(changes_fn, "w");
   if (changes_file == 0) {
 printf (SEMA_006, changes_fn);
 *changes_fn = '\0';
   } else {
 observe_changes = TRUE;
 printf (SEMA_009, changes_fn);
   }
 }
 */
}

void AutomataFactory::add_turing_rule (int st, int si, int nst, int nsi, int d)
{
	qDebug() << "add turing rule";
	if (turing != NULL)
	{

	}
	turing->addRule (st, si, nst, nsi, d);
}

int AutomataFactory::get_statenumber (char *vname)
{
	//	int i;
	//	char temp[100];

	qDebug() << "get_statenumber: suche Zustandsnamen " << vname;

	/*
 for (i=0; i<=max_states; i++)
 {
  if (!strcmp(state_array[i].name, vname))
  {
   // We have found an state
   return i;
  }
 }
 sprintf (temp, SEMA_011, vname);
 cal_error (temp);
 */
	return (-1);
}

void AutomataFactory::lastSettings ()
{
	qDebug() << "finish parsing";
	automata->finishInit();
	automataField->finishInit();
	automata->setField(automataField);
	automata->setStateList(automataStates);
}
