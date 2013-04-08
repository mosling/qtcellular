#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <QDebug>
#include "CQueue.h"
#include "CExpression.h"
#include "Random.h"

#define ready            0
#define division_by_zero 1
#define negative_log     2
#define mul_overflow     3
#define div_overflow     4
#define exp_overflow     5
#define unknown_funct    6
#define stack_underflow  7

QString error_array[8] = {
	"Ready", "division by zero", "negative log", "multiplication overflow",
	"division overflow", "exp overflow", "unknown function",
	"Stack underflow"
};

#define max_op          19

static char op[max_op][7] = {
	"AND", "OR", "XOR",
	"LE", "LT", "EQ", "NE", "GT", "GE", 
	"+", "-", "%",
	"*", "/", 
	"RND", "STATE", "SUM", "ABS",
	"^"
};

/*
static long op_pr[max_op] = {
	0,0,0, 
	1,1,1,1,1,1, 
	2,2,2, 
	3,3, 
	4,4,4,4, 
	5
};
*/

static long values[max_op] = {
	2,2,2,
	2,2,2,2,2,2,
	2,2,2,
	2,2,
	1,1,1,1,
	2
};

CExpression::CExpression() : tree(NULL)
{
}

CExpression::~CExpression()
{

}

QString CExpression::getError()
{
	return exp_error;
}

void CExpression::showTreeInternal (treeatom *tree, QString &resStr)
{
	bool showBrace = false;

	if (tree->left != NULL) 
		showTreeInternal (tree->left, resStr);
	switch (tree->what) {
		case 1 : resStr += QString("var(%1) ").arg(tree->value);
			break;
	case 2 : resStr += QString("%1 ").arg(tree->value);
			break;
		case 3 :
		showBrace = values[tree->value] == 1;
		resStr += QString("%1%2").arg(op[tree->value]).arg(showBrace?"(":" ");
	}
	if (tree->right != NULL) 
		showTreeInternal (tree->right,resStr);

	if (showBrace) resStr += ") ";
}

void CExpression::show()
{
	QString str;
	showTreeInternal (tree, str);
	qDebug() << str;
}

void CExpression::clearTreeInternal(treeatom *tree)
{
	if (tree->left != NULL)
		clearTreeInternal (tree->left);
	if (tree->right != NULL)
		clearTreeInternal (tree->right);
	free (tree);
}

void CExpression::clear()
{
	clearTreeInternal(tree);
	tree = NULL;
}


void CExpression::importQueueInternal(CQueue *expr_queue, treeatom **tree)
{
	cell *temp;

	temp = expr_queue->getHead();
	*tree = new treeatom;
	(*tree)->left = NULL;
	(*tree)->right = NULL;
	(*tree)->what = temp->type;
	(*tree)->value = temp->value;
	/* loeschen des Kopfes der Schlange */
	expr_queue->dequeue();
	if (((*tree)->what == 3) && (values[(*tree)->value]>0)) {
		/* garantiert einen Operanden */
		importQueueInternal (expr_queue, &((*tree)->right));
		if (values[(*tree)->value]==2)
			importQueueInternal (expr_queue, &((*tree)->left));
	}
}

void CExpression::importQueue(CQueue *expr_queue)
{
	if (NULL != tree) clear();
	importQueueInternal(expr_queue, &tree);
}

int CExpression::power (int a, int p)
{
	if (p==0) 
		return 1;
	else 
		for (;p>1; a *= a, p--);
	return a;
}

int CExpression::computeTreeInternal(treeatom *tree)
{
	treeatom *left,*right;
	int vResult;

	if (tree->what == 2) {
		/* konstanter Wert */
		return (tree->value);
	}
	else if (tree->what == 1) {
		/* Variable */
		return varList.at(tree->value);
	}
	else {
		left = tree->left;
		right = tree->right;
		switch (tree->value) {
			case 0 /* AND */:
				if (computeTreeInternal(left))
					return (computeTreeInternal(right));
				return (0);
			case 1 /* OR */:
				if (computeTreeInternal(left))
					return (1);
				return (computeTreeInternal(right));
			case 2 /* XOR */:
				if (computeTreeInternal(left))
					return (!computeTreeInternal(right));
				else
					return (computeTreeInternal(right));
			case 3 /* <= */:
				return (computeTreeInternal(left)<=computeTreeInternal(right)?1:0);
			case 4 /* < */:
				return (computeTreeInternal(left)<computeTreeInternal(right)?1:0);
			case 5 /* = */:
				return (computeTreeInternal(left)==computeTreeInternal(right)?1:0);
			case 6 /* != */:
				return (computeTreeInternal(left)!=computeTreeInternal(right)?1:0);
			case 7 /* > */:
				return (computeTreeInternal(left)>computeTreeInternal(right)?1:0);
			case 8 /* >= */:
				return (computeTreeInternal(left)>=computeTreeInternal(right)?1:0);
			case 9 /* + */:
				return (computeTreeInternal(left)+computeTreeInternal(right));
			case 10 /* - */:
				return (computeTreeInternal(left)-computeTreeInternal(right));
			case 11 /* % */:
				return (computeTreeInternal(left)%computeTreeInternal(right));
			case 12 /* * */:
				return (computeTreeInternal(left)*computeTreeInternal(right));
			case 13 /* / */:
				return (computeTreeInternal(left)/computeTreeInternal(right));
			case 14 /* RND */:
				return (my_random(computeTreeInternal(right)));
			case 15 /* STATE */:
				return currentField->getNeighborState(computeTreeInternal(right));
			case 16 /* SUM */:
				vResult = computeTreeInternal(right);
				return (currentField->getNeighborSum(true).at(vResult));
			case 17 /* ABS */:
				return (abs(computeTreeInternal(right)));
			case 18 /* ^ */:
				return (power(computeTreeInternal(left),computeTreeInternal(right)));
			default:
				exp_error = unknown_funct;
		}/* switch */
	}
	/* falls was schiefl"auft, gib Null zur"uck */
	return 0;
}

int CExpression::compute(Field *aField, QList<int> &vVars)
{
	currentField = aField;
	varList = vVars;
	return computeTreeInternal(tree);
}

