/*doc
Modell einer Turingmaschine 
*/

#include "Automata.h"
#include "TuringRule.h"

class Turing: public Automata
{
public:
	Turing();
	virtual ~Turing();
	virtual QString toString() const { return "2D Turing Machine"; }
	virtual void addRule(int st, int si, int nst, int nsi, int d);
	virtual void nextConfiguration ();
	virtual bool isStable() const;
	virtual void finishInit();

private:
	QList<TuringRule> ruleList;
	int xpos;
	int ypos;
	int direction;
	int state;
};
