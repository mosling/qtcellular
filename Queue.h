#ifndef CQUEUE_H
#define CQUEUE_H

#include <QString>
#include <QList>

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
 * Bereitstellen eines FIFO-Speichers mit speziellen Knoten zur
 * Aufnahme der Werte,Operatoren und Variablen während des Parsingprozesses
 * eines Ausdrucks.
 */
class Queue
{
private:
  cell *head, *tail;

public:
    Queue();
    ~Queue();
    void append (Queue *aQueue);
	void enqueue (int t, int v);
	void enqueue (cell *aCell);
	void dequeue ();
	void reverse();
	cell *getHead ();
	bool isempty ();
	void clear ();
	void show();
    QList<QString> tokenizeExpression(QString expression);
    void parseExpression(QString expression);
};

#endif
