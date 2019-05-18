#ifndef CQUEUE_H
#define CQUEUE_H

#include <QString>
#include <QList>

typedef struct cell {
	int type;
	int value;
	struct cell *next;
} cell;

/*!
 * This queue stores the values from the parser to later extract the
 * expression tree for computing new values for each cell.
 */
class Queue
{
private:
  cell *head, *tail;

public:
    Queue();
    ~Queue();
    cell *getHead ();
    bool isempty ();
    void clear ();
    void append (Queue *aQueue);
	void enqueue (int t, int v);
	void enqueue (cell *aCell);
	void dequeue ();

    void show();
	void reverse();

    QList<QString> tokenizeExpression(QString expression);
    void parseExpression(QString expression);
};

#endif
