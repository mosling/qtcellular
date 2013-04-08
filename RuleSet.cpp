#include <QDebug>
#include "RuleSet.h"

RuleSet::RuleSet(void)
{
}

RuleSet::~RuleSet(void)
{
	qDeleteAll(mRuleList);
	mRuleList.clear();
	mVarMap.clear();
	mVarList.clear();
}

void RuleSet::addRule(Rule *aRule)
{
	mRuleList.append(aRule);
}

int RuleSet::setVariable(QString aName, int aValue)
{
	int vIdx = getVarIndex(aName);

	if (-1 == vIdx)
	{
		vIdx = mVarList.size();
		mVarMap[aName] = vIdx;
		mVarList.append(aValue);
	//	qDebug() << "new variable '" << aName << "' = " << aValue << " at index " << vIdx;
	}
	else
	{
		mVarList[vIdx] = aValue;
	//	qDebug() << "change variable '" << aName << "' = " << aValue;
	}

	return vIdx;
}

void RuleSet::setVariable(int aIdx, int aValue)
{
	if (aIdx >= 0 && aIdx < mVarList.size())
	{
		mVarList[aIdx] = aValue;
	}
}

bool RuleSet::isVariable(QString aName)
{
	return mVarMap.contains(aName);
}

QString RuleSet::getVarName(int aIdx)
{
	QMapIterator<QString, int> it(mVarMap);
	while (it.hasNext())
	{
		it.next();
		if (it.value() == aIdx) return it.key();
	}

	return "??";
}

int RuleSet::getVarIndex(QString aName)
{
	if (isVariable(aName))
	{
		return mVarMap[aName];
	}
	return -1;
}

int RuleSet::getVarByName(QString aName)
{
	return mVarList.at(mVarMap[aName]);
}

int RuleSet::getVarByIndex(int aIdx)
{
	return mVarList.at(aIdx);
}

bool RuleSet::compute(Field *aField, int &aResult)
{
	Rule *r;
	int res = 0;
	bool rule_found = false;

	if (mRuleList.size() > 0)
	{
		QListIterator<Rule*> it(mRuleList);
		while(it.hasNext() && !rule_found)
		{
			r = it.next();
			rule_found = r->compute(aField, mVarList, res);
		}
	}
	else
	{
		qDebug() << "no existing rule ...";
	}
		aResult = res;
		return rule_found;
	}
