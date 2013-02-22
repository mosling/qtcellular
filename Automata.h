/*!
Abstrakte Klasse f�r Modelle, diese stellen im wesentlichen eine
Funktion bereit, f�r das Berechnen der n�chsten Konfiguration.
Dar�ber hinaus werden Funktionen zur �berwachung von
Ver�nderungen als inline-Funktionen implementiert. So rufen die
entsprechenden Funktionen des Feldes diese Observer auf.
*/

#ifndef MODEL_H
#define MODEL_H
#include <QPoint>
#include <QString>
#include <QHash>
#include "RuleSet.h"
#include "StateList.h"
#include "Field.h"
#include "global.h"

class Automata
{
public:
	Automata();
	virtual ~Automata();

	virtual QString toString () const = 0;
	virtual void nextConfiguration () = 0;
	virtual void finishInit () = 0;

	virtual void setParameter (QString aName, int aValue);
	virtual int getParameter (QString aName);

	virtual void setField(Field *aField);
	virtual Field *getField() const;

	virtual void setRuleSet(RuleSet *aRules);
	virtual RuleSet *getRuleSet() const;

	virtual void setStateList (StateList *sl);
	virtual StateList *getStateList() const;

	virtual bool isStable() const;

	void setCounter(quint64 v) { counter = v; }
	quint64 getCounter() { return counter; }
	void incCounter() { counter += 1; }

protected:
	bool stableConfiguration;
	Field     *field;
	RuleSet   *mRules;
	StateList *states;
	QHash<QString, int> mParameter;

private:
	quint64 counter;
};

#endif
