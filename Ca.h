#ifndef CA_H
#define CA_H

#include "Automata.h"


//! Diese Klasse implementiert einen zellul�ren Automaten.
class Ca : public Automata
{
public:
	Ca();
	virtual ~Ca();
	virtual QString toString () const { return "Cellular Automata"; }
	virtual void nextConfiguration ();
	virtual void finishInit ();

private:
	CellType changeCell();

	void nextSolid ();
	void nextMargolus (qint32 offset);

	void nextChecker ();
	void nextPlanar ();

	int lookup_table_exist;
	CellType *lookup_table;
	void create_lookup_table ();
};
#endif
