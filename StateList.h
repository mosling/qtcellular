#ifndef STATELIST_H
#define STATELIST_H

#include <QString>
#include <QColor>
#include <QMap>

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

class StateList
{
public:
    StateList();
    ~StateList();
    void addState(int what, QString name, int num, char c, QString s,int l, QString s2);
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
