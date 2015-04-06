#include <QDebug>

#include "CasimXmlParser.h"
#include "AutomataFactory.h"

#include <QFile>

CasimXmlParser::CasimXmlParser()
{
}

bool CasimXmlParser::parseAutomataFile(QString aFilename, AutomataFactory *af)
{
    factory = af;

    QDomDocument doc("CasimAutomata");
    QFile file(aFilename);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failure open file : " << aFilename;
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "error load xml: " << errorStr << " at line " << errorLine << " column " << errorColumn;
        file.close();
        return false;
    }

    parseElement("", doc.firstChild());

    file.close();

    return false;
}


void CasimXmlParser::parseElement(QString xpath, QDomNode aDomNode)
{
    QString vName, vValue;
    QDomElement vElem;
    QString basePath = xpath;

    while (!aDomNode.isNull())
    {
        vElem = aDomNode.toElement();
        vName = aDomNode.nodeName();
        vValue = aDomNode.nodeValue();

        switch (aDomNode.nodeType())
        {
            case QDomNode::ElementNode:
                if (vElem.firstChild().nodeName() == "#text")
                {
                    QString vText = vElem.firstChild().nodeValue();
                    qDebug() << "XPath" << basePath << ":" << vName << " is " << vText;
                    addNode(basePath, vName, vText)
                }
                else
                {
                    parseElement(basePath+"."+vName, vElem.firstChild());
                }
                break;
            case QDomNode::AttributeNode:
                qDebug() << "AttributeNode";
                break;
            case QDomNode::TextNode:
                qDebug() << "TextNode";
                break;
            case QDomNode::CDATASectionNode:
                qDebug() << "CDATASectionNode";
                break;
            case QDomNode::EntityReferenceNode:
                qDebug() << "EntityReferenceNode";
                break;
            case QDomNode::EntityNode:
                qDebug() << "EntityNode";
                break;
            case QDomNode::ProcessingInstructionNode:
                {
                    QString d;
                    d = aDomNode.toProcessingInstruction().data();
                    qDebug() << "ProcessingInstruction" << d;
                }
                break;
            case QDomNode::CommentNode:
                qDebug() << "CommentNode";
                break;
            case QDomNode::DocumentNode:
                qDebug() << "DocumentNode";
                break;
            case QDomNode::DocumentTypeNode:
                qDebug() << "DocumentTypeNode";
                break;
            case QDomNode::DocumentFragmentNode:
                qDebug() << "DocumentFragmentNode";
                break;
            case QDomNode::NotationNode:
                qDebug() << "NotationNode";
                break;
            case QDomNode::BaseNode:
                qDebug() << "BaseNode";
                break;
            case QDomNode::CharacterDataNode:
                qDebug() << "CharacterDataNode";
                break;
            default:
                vName = aDomNode.nodeName();
                qDebug() << "Default: " << vName ;
        }
        aDomNode = aDomNode.nextSibling();
    }
    qDebug() << "finish node " << basePath;
    finishNode(basePath);
}

void CasimXmlParser::addNode(QString basePath, QString vName, QString vText)
{
    if (basePath.endsWith("category"))
    {
        if ("type" == vName) factory->
    }
}

void CasimXmlParser::finishNode(QString basePath)
{

}



