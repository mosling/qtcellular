#ifndef CASIMXMLPARSER_H
#define CASIMXMLPARSER_H

#include <QString>
#include <QDomDocument>

class AutomataFactory;

class CasimXmlParser
{

private:
    AutomataFactory *factory;

    void parseElement(QString xpath, QDomNode aDomNode);
    void addNode(QString basePath, QString vName, QString vText);
    void finishNode(QString basePath);

public:
    CasimXmlParser();

    bool parseAutomataFile(QString name, AutomataFactory *af);
};

#endif // CASIMXMLPARSER_H
