﻿#ifndef RULE_H
#define RULE_H

#include <QList>
#include "CExpression.h"
#include "Field.h"

enum etRulePart
{
	RP_ACTION, RP_CONDITION, RP_MOVE1, RP_MOVE2
};


//! Diese Klasse verwaltet die Regeln eines einzelnen Modells
//! und einige weitere Parameter. Die Variablen dieser Klasse
//! werden durch den Parser gefüllt.
class Rule
{
public:
	Rule(void);
	~Rule(void);

	void addExpression(CExpression *aExp, etRulePart aPart);
	bool compute(Field *aField, QList<int> &vVars, int &aResult);

private:
	CExpression *action, *cond, *m1, *m2;

};

#endif
