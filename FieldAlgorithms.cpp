#include "FieldAlgorithms.h"
#include "Random.h"

FieldAlgorithms::FieldAlgorithms()
{
}

void FieldAlgorithms::Randomize(Field *field, StateList *states)
{
	int s=0, r=0, i=0;
	int max_states = states->size();
    qint32 c;

	QList<int> lhList;
	QList<StateListNode*> sl = states->getList();
	foreach (StateListNode* sln, sl)
	{
		s += sln->likelyhood;
		lhList.append(s);
	}

	field->ClearMask();
    field->SetFirstCell(Field::ITER_ALL, 0);
	while (!field->LastCell())
	{
		if (s==0)
		{
            c = my_random (max_states);
		}
		else
		{
			r = my_random(s);
			c = 0; i = 0;
			while (i < max_states)
			{
				if (r <= lhList[i] )
				{
					c = i;
					break;
				}
				i++;
			}
		}

		field->SetCellState(c);
        field->MarkCellMask(Field::CELL_CHANGED);
		field->NextCell();
	}
}
