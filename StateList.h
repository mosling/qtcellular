#ifndef STATELIST_H
#define STATELIST_H

#include <QString>
#include <QColor>
#include <QMap>

//! This internal class contains all information
//! about a possible state of a cell. The cell
//! state itself is a number only.
class StateListNode
{

public:
    unsigned char how;
    unsigned char likelyhood;
    QColor color;
    QString picname;
    QColor foreground;
    QColor background;
    char character;
};

//! The list of all possible states for the cells. This class
//! has also the mapping from name to number, so the user can
//! use the name of the defined cell to describe the rules.
class StateList
{
public:
    StateList();
    ~StateList();
    void addState(int what, QString name, char c, QString s, int l, QString s2);
    int getIndex(QString name);
    int size() const { return stateList.size(); }
    StateListNode *getState(int i);
    QList<StateListNode*> &getList() { return stateList; }
    void setMouseState(int aMouseState);
    int getMouseState() const;

private:
    QMap<QString, int> stateNameMap;
    QList<StateListNode*> stateList;
    int mStateIndex;
    int mouseState;

};

#endif // STATELIST_H
