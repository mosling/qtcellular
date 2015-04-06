#include <string>
#include <QtGlobal>
#include <QtDebug>
#include <QMap>
#include <QStack>
#include "Queue.h"

Queue::Queue()
{
	head = NULL;
	tail = NULL;
}

Queue::~Queue()
{
	clear();
}

/*
Einfügen eines Werte in die Queue. Dies geschieht nach ADT am Ende 
der Liste.
\param t Typ des Knotens
\param v ein Wert, (Konstante, Operatorindex oder Variablenindex)
*/
void Queue::enqueue(int t, int v)
{
	cell *temp;

	temp = new cell();
	temp->type = t;
	temp->value = v;
	temp->next = NULL;
	enqueue (temp);
}

void Queue::enqueue(cell *aCell)
{
	if (tail != NULL) tail->next = aCell;
	else head = aCell;

	tail = aCell;
}

void Queue::append(Queue *aQueue)
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
void Queue::dequeue()
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
cell *Queue::getHead()
{
	return head;
}

/*!
Umdrehen der Einträge.
*/
void Queue::reverse ()
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
bool Queue::isempty()
{
	return (head == NULL);
}

/*!
Löschen aller Elemente der Queue.
*/
void Queue::clear()
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

void Queue::show()
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

QList<QString> Queue::tokenizeExpression(QString expression)
{
    QList<QString> result;
    QString currentToken;
    QMap<QString,QString> conversionMap;
    conversionMap["<="] = "LE";
    conversionMap["<"] = "LT";
    conversionMap[">="] = "GE";
    conversionMap[">"] = "GT";
    conversionMap["!="] = "NE";
    conversionMap["="] = "EQ";
    conversionMap["=="] = "EQ";

    int tokenType = 0; // 1: Letter, 2: Digit, 3: Space, 4: Klammern 5: Operator (all other)

    foreach (QChar c, expression )
    {
        for (int charType = 1; charType <= 5; ++charType)
        {
            bool isType = false;
            switch (charType)
            {
            case 1: isType = c.isLetter(); break;
            case 2: isType = c.isDigit(); break;
            case 3: isType = c.isSpace(); break;
            case 4: isType = c == '(' || c == ')'; break;
            default: isType = true;
            }

            if (isType)
            {
                if ( charType == 3 || (tokenType == 1 && charType != 1 && charType != 2) || (tokenType != 1 && tokenType != charType) )
                {
                    if (currentToken.size() > 0)
                    {
                        result.append(conversionMap.contains(currentToken) ? conversionMap[currentToken] : currentToken);
                        currentToken = "";
                    }
                    tokenType = charType;
                }
                if (charType != 3)
                {
                    currentToken += c;
                }
                break;
            }
        }
    }

    if (currentToken.length() > 0 )
    {
        result.append(currentToken);
    }

    return result;
}

void Queue::parseExpression(QString expression)
{
    static QList<QString> operatorList;
    operatorList << "AND" << "OR" << "XOR"
                 << "LE" << "LT" << "EQ" << "NE" << "GT" << "GE"
                 << "+" << "-" << "%" << "*" << "/" << "^";

    static QList<QString> operatorAssociation;
    operatorAssociation << "L" << "L" << "L"
                        << "L" << "L" << "L" << "L" << "L" << "L"
                        << "L" << "L" << "R" << "L" << "L" << "R";

    // higher precedence means computation
    static QList<int> operatorPrecedence;
    operatorPrecedence << 1 << 1 << 1
                       << 2 << 2 << 2 << 2 << 2 << 2
                       << 3 << 3 << 4 << 4 << 4 << 5;

    static QList<QString> functionList;
    functionList << "RND" << "STATE" << "SUM" << "ABS";

    static QList<QString> structuralElements;
    structuralElements << "(" << ")" << ",";

    qDebug() << "parse Expression" << expression;

    expression = expression.toUpper();
    expression = expression.replace("CELL", "STATE(0)");
    QList<QString> tokenList = tokenizeExpression(expression);

    int tokenType = 0; // 1: Variable, 2: Number, 3: Operator

    foreach (QString t, tokenList)
    {
        bool isNumber = false;
        t.toInt(&isNumber);

        if (isNumber) { tokenType = 2; }
        else if (operatorList.contains(t)){ tokenType = 3; }
        else if (functionList.contains(t)){ tokenType = 4; }
        else { tokenType = 1; }

        qDebug() << tokenType << ": " << t;
    }

    QList<QString> output;
    QStack<QString> stack;
//    Implement the https://en.wikipedia.org/wiki/Shunting-yard_algorithm from Dijkstra
//    While there are tokens to be read:
//    Read a token.
    foreach (QString token, tokenList)
    {
//    First some pre computation
        int isOperator = operatorList.indexOf(token);
        bool isFunction = functionList.contains(token);
        bool isStructural = structuralElements.contains(token);
//    If the token is a number, then add it to the output queue.
        if (isOperator == -1 && !isFunction && !isStructural)
        {
            output.append(token);
        }
//    If the token is a function token, then push it onto the stack.
        else if (isFunction)
        {
            stack.push(token);
        }
//    If the token is a function argument separator (e.g., a comma):
        else if (token == ",")
        {
//    Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue. If no left parentheses are encountered, either the separator was misplaced or parentheses were mismatched.
            while (stack.top() != "(")
            {
                output.append(stack.pop());
            }
        }
//    If the token is an operator, o1, then:
        else if (isOperator != -1)
        {
            bool next = true;
//    while there is an operator token, o2, at the top of the operator stack, and either
            while (next && stack.size() > 0)
            {
                next = false;
                int o2 = operatorList.indexOf(stack.top());
                if ( o2 != -1)
                {
//    o1 is left-associative and its precedence is less than or equal to that of o2, or
//    o1 is right associative, and has precedence less than that of o2,
                    QString o1A = operatorAssociation[isOperator];
                    int o1P = operatorPrecedence[isOperator];
                    int o2P = operatorPrecedence[o2];
                    if ( (o1A == "L" && o1P <= o2P) || (o1A == "R" && o1P < o2P) )
                    {
                        //    then pop o2 off the operator stack, onto the output queue;
                        output.append(stack.pop());
                        next = true;
                    }
                }
            }
//    push o1 onto the operator stack.
            stack.push(token);
        }
//    If the token is a left parenthesis, then push it onto the stack.
        else if (token == "(")
        {
            stack.push(token);
        }
//    If the token is a right parenthesis:
        else if (token == ")")
        {
//    Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
            while (stack.top() != "(")
            {
                output.append(stack.pop());
            }
//    Pop the left parenthesis from the stack, but not onto the output queue.
            stack.pop();
//    If the token at the top of the stack is a function token, pop it onto the output queue.
            if (functionList.contains(stack.top())) { output.append(stack.pop()); }
//    If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
        }
    }
//    When there are no more tokens to read:
//    While there are still operator tokens in the stack:
//    If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses.
//    Pop the operator onto the output queue.
    while (stack.size() > 0)
    {
        if (stack.top() == "(") qDebug() << "mismatched parenthesis";
        output.append(stack.pop());
    }
//    Exit.

    foreach (QString t, output)
    {
        qDebug() << t;
    }
}
