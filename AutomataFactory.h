#ifndef AUTOMATAFACTORY_H
#define AUTOMATAFACTORY_H

#include <QWidget>
#include <QMap>
#include "Queue.h"
#include "StateList.h"
#include "RuleSet.h"
#include "Grid.h"
#include "Ca.h"
#include "Turing.h"

enum etAutomataType
{
	TURINGMODEL, CAMODEL, CA1DMODEL, AGENTMODEL
};

/*!
  Diese Klasse stellt die Schnittstelle zwischen der Beschreibungssprache und
  den konkreten Objekten her. Hier werden die richtigen Objekte erzeugt und
  mit Werten gefüllt:
 */
class AutomataFactory
{
public:
	AutomataFactory ();
	~AutomataFactory();
	void setAutomataName (QString aName, etAutomataType aType);
	void setSteps (int,int);
	void setArray (int,int);
	void addState(int what, QString name, int num, char c, QString s,int l, QString s2);
	void add_gridcolor (char *name);
	void create_states (int what,int n,char *name,int c,int l);
	void addNeumannNeighborhood(int n);
	void addMooreNeighborhood(int n);
	void addRadiusNeighborhood (int n);
	void addPoint(int x,int y);
	void addMargolusNeighborhood ();
	void addPlanarmap (char *fname);
	void setBorderType(int bt, int ptb);
	void setParameter(QString aName, int aValue);
	int  setVariable(QString aName, int aValue);
    void setFunction(etRulePart aFuncType, Queue *aQueue);
	void setFieldSquare (int x,int y,int w,int c);
	void loadField (QString aFileName, bool isAscii);
	void randomField ();
	void set_observer (int which);
	void init();
	void lastSettings();
	void addTuringFour ();
	void addTuringEight ();
	void add_turing_rule (int st, int si, int nst, int nsi, int d);
    void addStateOrVariable (QString vName, Queue *aQueue);
	int  get_statenumber (char *vname);
	QWidget *getFieldWidget() { return automataFieldWidget; }
	Automata* getAutomata() { return automata; }

private:
	Automata  *automata;
	StateList *automataStates;
	RuleSet   *mRules;
	Rule      *mCurrentRule;
	Field     *automataField;
	Grid      *gridField;
	QWidget   *automataFieldWidget;
	Turing    *turing;
};

#endif
