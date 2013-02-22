#include "numericvalidator.h"

NumericValidator::NumericValidator(QObject *parent)
   : QValidator(parent), min(0), max(0)
{
}

NumericValidator::NumericValidator(int bottom, int top, QObject *parent)
   : QValidator(parent), min(bottom), max(top)
{
}

NumericValidator::~NumericValidator()
{
}

int NumericValidator::convertToInt(bool *bOk, QString str) const
{
   int value = 0;

   if (str.endsWith("H"))
   {
      value = str.left(str.size()-1).toInt(bOk, 16);
   }
   else
   {
      value = str.toInt(bOk);
   }

   return value;
}

void NumericValidator::setBottom(QString bottom)
{
   bool ok;
   min = convertToInt(&ok, bottom);
}

void NumericValidator::setTop(QString top)
{
   bool ok;
   max = convertToInt(&ok, top);
}

/*!
Beim Prüfen wird die Zeichenkette mit dem neuen Zeichen als Parameter
übergeben. Wenn die Methode Invalid zurückgibt, wird das Zeichen nicht
in den LineEditor übernommen.
TODO Bisher wird nur ein positiver Bereich abgedeckt.
*/
QValidator::State NumericValidator::validate(QString& input, int& pos) const
{
   QValidator::State retState = QValidator::Invalid;

   input = input.trimmed();
	if (input.size() == 0)
	{
		retState = QValidator::Intermediate;
	}
	else
	{
		bool ok;
		int value = convertToInt(&ok, input);

		if (ok && (min != 0 || max != 0))
		{
			if ( (min >= 0 && value >= 0) || (max <= 0 && value <= 0) )
			{
				if (value < min) retState = QValidator::Intermediate;
				else if (value >= min && value <= max) retState = QValidator::Acceptable;
			}
		}
	}
   return retState;
}

void NumericValidator::fixup(QString& input) const
{
   input = input.trimmed();
	if (input.length() == 0) input = "0";
}
