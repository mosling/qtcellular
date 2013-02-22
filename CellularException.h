#ifndef CELLULAREXCEPTION_H
#define CELLULAREXCEPTION_H

#include <QString>

class CellularException
{
public:
	 CellularException(QString aFile, int aLine, QString aWhat);
	 const QString explain() const throw();
private:
	 QString fileName;
	 int lineNumber;
	 QString text;

};

#endif // CELLULAREXCEPTION_H
