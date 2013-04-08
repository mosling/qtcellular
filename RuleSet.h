#ifndef RULESET_H
#define RULESET_H

#include <QList>
#include <QMap>
#include <QString>
#include "Rule.h"


//! Diese Klasse verwaltet die Regeln eines einzelnen Modells
//! und einige weitere Parameter. Die Variablen dieser Klasse
//! werden durch den Parser gefüllt.
class RuleSet
{
public:
	RuleSet(void);
	~RuleSet(void);

	// Regelsätze,
	void addRule(Rule *aRule);
	bool compute(Field *aField, int &aResult);

	// variable handling
	int  setVariable(QString aName, int aValue);
	void setVariable(int aIdx, int aValue);
	bool isVariable(QString aName);
	QString getVarName(int aIdx);
	int  getVarIndex(QString aName);
	int  getVarByName(QString aName);
	int  getVarByIndex(int aIdx);

private:
	QMap<QString, int> mVarMap;
	QList<int> mVarList;
	QList<Rule*> mRuleList;

};
#endif
