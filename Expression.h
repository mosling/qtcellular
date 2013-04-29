#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#include <QString>
#include "Field.h"
#include "Queue.h"

//! Simple Tree Node wich contains pointer to the left and right child
//! nodes. Which kind
typedef struct treeatom {
	struct treeatom *left;
	struct treeatom *right;
	int what;
	int value;
} treeatom;

//! This class saves a numerical expression as expression tree.
class Expression
{
public:
    Expression();
    ~Expression();

	int compute (Field *aField, QList<int> &vVars);
	void show ();
	QString getError ();
	char *equation_error_text();
	void clear ();
    void importQueue (Queue *expr_queue);

	unsigned char number (char *t, int *d);
	int value (char *t);

private:
	treeatom *tree;
	int dummy;	 /* Dummy-Variable f"ur Number-aufrufe */
	QString exp_error;
	Field *currentField;
	QList<int> varList;

	int  power (int a, int p);
	int  computeTreeInternal (treeatom *tree);
	void showTreeInternal (treeatom *tree, QString &resStr);
	void clearTreeInternal(treeatom *tree);
    void importQueueInternal(Queue *expr_queue, treeatom **tree);

};

#endif
