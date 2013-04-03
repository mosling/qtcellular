#include <QDebug>
#include <stdlib.h>
#include <math.h>
#include "Field.h"
#include "random.h"
#include "Grid.h"
#include "Ca.h"
#include "CellularException.h"

Ca::Ca() : Automata(),
	lookup_table_exist(0),
	lookup_table(NULL)
{
	mParameter["NeedPaint"] = 1;
	mParameter["NeedUpdate"]= 1;
	mParameter["NeedStart"] = 0;
	mParameter["NeedDirection"] = 0;
}

Ca::~Ca()
{
}

void Ca::nextConfiguration ()
{
	try
	{
		stableConfiguration = true;
		field->ClearMask ();
		mRules->setVariable("ARAND",my_random(100));
		switch (getParameter("NEWCELLMETHOD"))
		{
		case 0: // alle Zellen betrachten
			nextSolid();
			break;
		case 3: // margolus Feld
			nextMargolus(static_cast<qint32>(getCounter()%2));
			break;
		default:
			throw (new CellularException(__FILE__,__LINE__,"unknown new cell method"));
			break;
		}
		incCounter();
		field->updateField();
	}
	catch (CellularException &e)
	{
		qDebug() << e.explain();
	}

	/*
 switch (new_cell_method) {
  case 0: // if solid new
	solid_cellarray ();
	break;
  case 1: // if checker new
	checker_cellarray ();
	break;
  case 3: // if margolus new
	margolus_cellarray ();
	break;
  case 5: // cells are not grids
	planar_cellarray ();
	break;
 }
 */
}


//! Neuen Wert für die aktuelle Zelle berechnen. Die aktuelle Zelle
//! wird durch das aktive Feld festgelegt. Wenn eine Regel für die aktuelle
//! Umgebung existiert, wird der neue Zellwert bestimmt. Unterscheidet
//! sich dieser vom bisherigen Wert wird das Maskenfeld gesetzt.
//! Ansonsten wird der vorhandene Wert übernommen.
qint32 Ca::changeCell()
{
	int res;
    qint32 newState;
	bool rule_found = false;

	rule_found = mRules->compute(this->field, res);

	if (rule_found)
	{
        newState = static_cast<qint32>(res);

		if (field->GetCellState() != newState)
		{
            field->MarkCellMask(Field::CELL_CHANGED);
			stableConfiguration = false;
		}
	}
	else
	{
		newState = field->GetCellState ();
	}
	field->SetNewState(newState);
	return newState;
}

//! Es werden alle Zellen durchlaufen und der nächste Zustand
//! berechnet. Der genaue Durchlauf der Zellen wird durch das
//! Feld bestimmt. Der Automat selbst kennt den Aufbau nicht.
void Ca::nextSolid ()
{
	field->SetFirstCell(Field::ALL, 0);
	while (!field->LastCell())
	{
		changeCell();
		field->NextCell();
	}
}

//! Neuberechnung der Zellen in eier Margolusumgebung
//! Der offset-Wert kann 0 oder 1 sein.
void Ca::nextMargolus (qint32 offset)
{
	field->SetFirstCell(Field::MARGOLUS, offset);
	mRules->setVariable("MRAND", my_random(100));
	while (!field->LastCell())
	{
		changeCell();
		field->NextCell();
		if (field->getNeighborhoodIndex()==0)
		{
			mRules->setVariable("MRAND", my_random(100));
		}
	}
}

//! Neuberechnung, wenn das Fels eine planarer Graph ist.
//! Abgebildet durch eine Zerlergung der Ebene.
void Ca::nextPlanar ()
{
	/*
 rule_record *f;
 int res=0;
 int rule_found;
 qint32 new_state;

 field->SetFirstCell ();
 while (!field->LastCell ())
 {
  field->ComputeSum (0,0);
  f = first_rule;
  rule_found = FALSE;
  while ((f != NULL) && !rule_found)
  {
	res = compute_tree (f->ftest);
	if (res != 0)
	{
	 if (f->flag == 0)
	 {
	  // nur normales setzen
	  res = compute_tree(f->faction);
      new_state = (qint32)(res);
#ifdef DEBUG_MODE
	  printf ("new_array.planar_...Set new state: %d\n", new_state);
#endif
	  field->SetNewState (new_state);
	  if (field->GetCellState() != new_state)
	  {
		field->fieldChanged = TRUE;
		field->MarkCellMask (CHANGED);
	  }
	  rule_found = TRUE;
	 }
	}
	f = f->next;
  }
  // zum Schlu"s die identische Regel
  // falls keine andere Regel gefunden wurde
  if (rule_found == FALSE)
  {
	res = field->GetCellState();
	field->SetNewState (res);
  }
  field->NextCell ();
 }
 */
}

void Ca::create_lookup_table ()
{
	/*
 int b,i,res, lookup_pos;
 unsigned int max_counter,counter;
 rule_record *f;
 qint32 new_state=0;
 unsigned char okend;

 max_counter = 1;
 if ((max_states==1) && ((max_neighbors)<12) &&
	(!moveable_cells) && (new_cell_method<3)) {
  for (i=0;i<=(max_neighbors);i++) max_counter *= 2;
  printf (NEWA_001,max_counter);
  // we can create an lookup_table or recreate
  if (! lookup_table_exist)
    lookup_table = (qint32 *)
     my_alloc (max_counter*sizeof(qint32));
  printf (NEWA_002, max_states+1, max_neighbors+add_cell);
  lookup_table_exist = 1;
  for (i=0;i<=max_neighbors;i++) pos_neighbors[i] = 0;
  // die ruhe-Regel wird dem Automatem "uberlassen
  // lookup_table[0] wird nicht mehr fest auf 0 gesetzt
  if (add_cell) b=0; else b=1;
  for (counter=1;counter<=max_counter;counter++)
  {
	memset((Anyptr)sum_neighbors, 0, MAXNEIGHBORS * sizeof(char));
	for (i=b;i<=max_neighbors;i++) sum_neighbors[pos_neighbors[i]]++;
	f = first_rule; okend=0;
	while (f != NULL)
	{
	 res = compute_tree (f->ftest);
	 if (res != 0) {
	  res = compute_tree(f->faction);
      new_state = (qint32)(res);
	  f = NULL;
	  okend = 1;
	 } else
	  f = f->next;
	}
	if (!okend) new_state=pos_neighbors[0];
	lookup_pos = 0;
	for (i = max_neighbors; i>=0; i--) {
	 lookup_pos = (lookup_pos + pos_neighbors[i]);
	 lookup_pos <<= 1;
	}
	lookup_pos >>=1;
	lookup_table[lookup_pos] = new_state;
	i = 0;
	pos_neighbors[i]++;
	while ((pos_neighbors[i]>max_states))
	{
	 pos_neighbors[i] = 0;
	 i++;
	 pos_neighbors[i]++;
	}
  }
 }
 printf ("fertig\n");
 */
}

//! Als einzige Aktion wird eine Lookup-Tabelle erzeugt,
//! wenn diese nicht zu groß wird.
void Ca::finishInit ()
{
	create_lookup_table();
}
