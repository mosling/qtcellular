#include "Automata.h"

Automata::Automata() :
	stableConfiguration(false),
	field(NULL),
	mRules(NULL),
	states(NULL),
	counter(0)
{
}

//! Hier werden die zugewiesene Teile auch wieder freigegeben.
Automata::~Automata()
{
	delete states; states = NULL;
	delete field; field = NULL;
	delete mRules; mRules = NULL;
}

void Automata::setRuleSet(RuleSet *aRules)
{
	mRules = aRules;
}

void Automata::setField(Field *aField)
{
	field = aField;
}

void Automata::setStateList(StateList *sl)
{
	states = sl;
}

RuleSet *Automata::getRuleSet() const
{
	return mRules;
}

Field *Automata::getField() const
{
	return field;
}

StateList *Automata::getStateList() const
{
	return states;
}

bool Automata::isStable() const
{
	return stableConfiguration;
}

void Automata::setParameter(QString aName, int aValue)
{
	mParameter[aName.toUpper()] = aValue;
}

int Automata::getParameter(QString aName)
{
	return mParameter[aName.toUpper()];
}
