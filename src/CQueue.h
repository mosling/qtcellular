#ifndef CQUEUE_H
#define CQUEUE_H

#include <QString>

/*!
Struktur für die Aufnahme der Daten eines Knotens
der Queue.
*/
typedef struct cell {
	int type;
	int value;
	struct cell *next;
} cell;

/*!
Bereitstellen eines FIFO-Speichers mit speziellen Knoten zur
Aufnahme der Werte,Operatoren und Variablen während des Parsingprozesses
eines Ausdrucks.
*/
class CQueue
{
private:
  cell *head, *tail;

public:
	CQueue();
	~CQueue();
	void append (CQueue *aQueue);
	void enqueue (int t, int v);
	void enqueue (cell *aCell);
	void dequeue ();
	void reverse();
	cell *getHead ();
	bool isempty ();
	void clear ();
	void show();
};

#endif
