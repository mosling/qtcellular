#include "Rule.h"

Rule::Rule(void) :
    action(NULL), cond(NULL), m1(NULL), m2(NULL)
{
}

//!
//! Die Regel-Funktionen werden aus dem AutomataFactory in
//! die Regel übernommen, wenn die Regel entfernt wird,
//! werden auch die Funktionen entfernt.
Rule::~Rule(void)
{
    delete action;
    delete cond;
    delete m1;
    delete m2;
}

void Rule::addExpression(Expression *aExp, etRulePart aPart)
{
    switch (aPart)
    {
    case RP_ACTION:
        action = aExp;
        break;
    case RP_CONDITION:
		  cond = aExp;
        break;
    case RP_MOVE1:
        m1 = aExp;
        break;
    case RP_MOVE2:
        m2 = aExp;
        break;
    }
}

bool Rule::compute(Field *aField, QList<int> &vVars, int &aResult)
{
	int c = 1;
	bool vRes = false;

	// bisherige Summe ungültig machen
	aField->getNeighborSum(false);
	if (NULL != cond)
	{
		c = cond->compute(aField, vVars);
	}

	if ( 0 != c)
	{
		Q_ASSERT(action != NULL);
		aResult = action->compute(aField, vVars);
		vRes = true;
	}

	return vRes;
}
