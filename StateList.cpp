#include "StateList.h"
#include <QtDebug>

StateList::StateList() : mStateIndex(0), mouseState(-1)
{
}

StateList::~StateList()
{
    qDeleteAll(stateList);
    stateList.clear();
	 stateNameMap.clear();
}

/*!
 \brief

 \param what
 \param name
 \param num
 \param c
 \param s
 \param l
 \param s2
*/
void StateList::addState(int what,QString name, int num,
                         char c, QString s,int l, QString s2)
{

    qDebug() << QString("Add State  %1,%2,%3,%4,%5,%6,%7")
                .arg(name).arg(what).arg(num).arg(c).arg(s).arg(l).arg(s2);

    StateListNode *node = NULL;
    int ind = getIndex(name);
    if (ind != -1)
    {
        node = stateList[ind];
    }
    else
    {
		 int idx = stateList.size();
		 node = new StateListNode();
		 stateList.append(node);
		 stateNameMap[name] = idx;
    }

    switch (what)
    {
    case 0: /* simple named color */
    case 4:
        node->how = 0;
        node->color.setNamedColor(s);
        qDebug() << "color is: " << node->color.name();
        break;
    case 1: /* loading picture */
        node->how = 1;
        node->picname = s;
        break;
    case 2: /* sign */
    case 5:
        node->how = 2;
        node->foreground.setNamedColor(s);
        node->background.setNamedColor(s2);
        node->character = c;
        break;
    case 3: /* number of state of cell */
        node->how = 3;
        break;
    }
    node->likelyhood = l;
}

int StateList::getIndex(QString name)
{
	if (stateNameMap.size()>0)
	{
		if (stateNameMap.contains(name))
			return stateNameMap[name];
	}
	return -1;
}

StateListNode *StateList::getState(int i)
{
	if (i>=0 && i<stateList.size())
	{
		return stateList.at(i);
	}
	return NULL;
}

void StateList::setMouseState(int aMouseState)
{
   mouseState = aMouseState;
}

int StateList::getMouseState() const
{
   return mouseState;
}
