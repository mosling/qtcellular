#include "Turing.h"
#include <QDebug>

Turing::Turing() : xpos(0), ypos(0), direction(0), state(-1)
{
	mParameter["DIRTYPE"] = 1;
}

Turing::~Turing()
{
	ruleList.clear();
}

void Turing::addRule(int st, int si, int nst, int nsi, int d)
{
	TuringRule r;
	r.state = st;
	r.sign = si;
	r.newSign = nsi;
	r.newState = nst;
	r.direction = d;

	ruleList.append(r);
}

void Turing::nextConfiguration ()
{
    qint32 sign;

	field->ClearMask();

	if (state == -1)
	{
		// Endzustand erreicht, da wird sich nichts dran ändern :-)
		stableConfiguration = true;
		return;
	}

	stableConfiguration = false;
	field->SetActiveCell (xpos, ypos);
	sign = field->GetCellState ();

	for (int i=0; i < ruleList.size(); ++i)
	{
		const TuringRule &vRule = ruleList.at(i);
		if ((vRule.sign == sign) && (vRule.state == state))
		{
			if (vRule.newState > -1)  // -1 Endzustand
			{
				/* put in newfield to have an correct state-observer */
				field->SetCellState(vRule.newSign);
                field->MarkCellMask (Field::CELL_CHANGED);

				/* Select absolut or relative directions */
				if (0 == mParameter["DIRTYPE"])
				{
					// Richtung in der Regel ist absolut
					direction = vRule.direction;
				} 
				else
				{
					// relative Angabe der Richtungänderung
					int maxn = field->getNeighborSize();
					direction = (direction + vRule.direction);
					if (direction < 0) direction = maxn + direction;
					else if (direction >= maxn) direction = direction - maxn;
				}
				field->MoveActiveCell(direction);
				field->GetActiveCell(xpos, ypos);
                field->MarkCellMask(Field::CELL_CHANGED);
			}
			state = vRule.newState;

			return;
		}
	}
}

bool Turing::isStable() const
{
	return (-1 == state);
}

/*!
Beim Aufruf dieser Methode werden die Werte aus den Parametern
einmalig in die Variablen übernommen.
*/
void Turing::finishInit()
{
	if (mParameter.contains("XPOS")) xpos = mParameter["XPOS"];
	if (mParameter.contains("YPOS")) ypos = mParameter["YPOS"];
	if (mParameter.contains("DIR")) direction = mParameter["DIR"];
	if (mParameter.contains("STATE")) state = mParameter["STATE"];
}

