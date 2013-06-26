#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QPoint>
#include <QString>
#include <QHash>
#include "RuleSet.h"
#include "StateList.h"
#include "Field.h"

//! Abstract class for all models. A model is the cetral point for
//! the simulation of this set of automata. The method nextConfiguration()
//! includes the computation of the next configuration based at the
//! Abstrakte Klasse für Modelle, diese stellen im wesentlichen eine
//! Funktion bereit, für das Berechnen der nächsten Konfiguration.
//! Darüber hinaus werden Funktionen zur Überwachung von
//! Veränderungen als inline-Funktionen implementiert. So rufen die
//! entsprechenden Funktionen des Feldes diese Observer auf.
class Automata : public QObject
{
    Q_OBJECT
    Q_CLASSINFO ("author", "skoehler")

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
