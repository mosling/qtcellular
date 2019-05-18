#include "CellularException.h"

CellularException::CellularException(QString aFile, int aLine, QString aWhat):
    fileName(aFile), lineNumber(aLine), text(aWhat)
{

}

const QString CellularException::explain() const
{
    static QString s;

    s = QString("%1 at %2 Exception: '%3'")
            .arg(fileName).arg(lineNumber).arg(text);

    return s;
}
