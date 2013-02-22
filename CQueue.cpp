#include <string>
#include <QtGlobal>
#include <QtDebug>
#include "CQueue.h"

CQueue::CQueue()
{
	head = NULL;
	tail = NULL;
}

CQueue::~CQueue()
{
	clear();
}

/*
Einfügen eines Werte in die Queue. Dies geschieht nach ADT am Ende 
der Liste.
\param t Typ des Knotens
\param v ein Wert, (Konstante, Operatorindex oder Variablenindex)
*/
void CQueue::enqueue(int t, int v)
{
	cell *temp;

	temp = new cell();
	temp->type = t;
	temp->value = v;
	temp->next = NULL;
	enqueue (temp);
}

void CQueue::enqueue(cell *aCell)
{
	if (tail != NULL) tail->next = aCell;
	else head = aCell;

	tail = aCell;
}

void CQueue::append(CQueue *aQueue)
{
	cell *temp;

	temp = aQueue->head;
	while (temp != NULL)
	{
		this->enqueue(temp);
		temp = temp->next;
	}
}

/*!
Löschen des ersten Eintrages der Queue. Es wird kein
Wert geliefert.
*/
void CQueue::dequeue()
{
	cell *save;

	if (head != NULL)
	{
		save = head;
		if (head == tail)
		{
			tail = NULL;
		}
		head = head->next;
		delete save ;
	}
}

/*!
Es werden die Werte des Kopfelemnetes abgefragt.
\return Zeiger auf den Kopfknoten
*/
cell *CQueue::getHead()
{
	return head;
}

/*!
Umdrehen der Einträge.
*/
void CQueue::reverse ()
{
	cell *temp,*memory,*swap;

	temp = head;
	swap = NULL;
	while (temp != NULL) {
		memory = temp->next;
		temp->next = swap;
		if (swap==NULL)
		{
			tail = temp;
		}
		swap = temp;
		temp = memory;
	}
	head = swap;
}

/*!
\return true, falls kein Elemnt enthalten ist.
*/
bool CQueue::isempty()
{
	return (head == NULL);
}

/*!
Löschen aller Elemente der Queue.
*/
void CQueue::clear()
{
	cell *temp, *otemp;

	temp = head;
	while (temp != NULL) {
		otemp = temp;
		temp = temp->next;
		delete otemp;
	}
	head=NULL;
	tail=NULL;
}

void CQueue::show()
{
	static char op[19][7] = {
		"AND", "OR", "XOR",
		"LE", "LT", "EQ", "NE", "GT", "GE",
		"+", "-", "%",
		"*", "/",
		"RND", "STATE", "SUM", "ABS",
		"^"
	};

	cell *temp;

	temp = head;
	while (temp != NULL)
	{
		switch (temp->type)
		{
		case 1:
			qDebug() << "V: " << temp->value;
			break;
		case 2:
			qDebug() << "C: " << temp->value;
			break;
		case 3:
			qDebug() << "O: " << op[temp->value];
			break;
		default:
			qDebug() << "unknown entry";
		}
		temp = temp->next;
	}
}
