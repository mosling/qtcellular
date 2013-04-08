#include "ParamDialog.h"
#include "NumericValidator.h"
#include <typeinfo>
#include <QLineEdit>
#include <QComboBox>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QLabel>
#include <QCheckBox>

ParamDialog::ParamDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ParamDialogWindowClass)
	 , wasCanceled(false), currRow(0)
{
    ui->setupUi(this);
	 gridLayout = new QGridLayout(ui->inpFrame);
}

ParamDialog::~ParamDialog()
{
    delete ui;
}

/*!
This function creates a entry at the user input dialog window. Depending
on the aValues different input types possible:
* start with '[' the method BitVectorInput::constructWidget() is used
* a comma seperated list is display as combo box
* if it start with '/' the regular expression validator is used
* if it contains a '-' a numeric validator with minimum and maximal value used
*/
void ParamDialog::addInputField(QString label, QString aValues, QString aCurrent)
{
	// special input method for Numbers is the Bitfield 
	QLabel *vLabel = new QLabel(label, ui->inpFrame);
	gridLayout->addWidget(vLabel,currRow, 0, 1, 1);

	if (!aValues.startsWith("/") && aValues.contains(",") )
   {
		// we have a list of predefined values, which should be used
      // create a combo box
		QComboBox *cb = new QComboBox(ui->inpFrame);
		QStringList cbe = aValues.split(",", QString::SkipEmptyParts);
		cb->addItems(cbe);
      formList.append(cb);
		int selIdx = 0;
		int idx = 0;
		foreach (QString str, cbe)
		{
			if (str == aCurrent) selIdx = idx;
			idx++;
		}
		cb->setCurrentIndex(selIdx);
		gridLayout->addWidget(cb, currRow, 1, 1, 1);
   }
   else
   {
		// all other cases creates a simple input line with the rigth validator
      QLineEdit *le = new QLineEdit(ui->inpFrame);
      if (aValues.startsWith("/"))
      {
			QString re = aValues.right(aValues.size()-1);
         le->setValidator(new QRegExpValidator(QRegExp(re), le));
			le->setToolTip(re);
      }
      else if (aValues.contains("-"))
      {
         QStringList sl = aValues.split("-", QString::SkipEmptyParts);
         if (sl.size() == 2)
         {
            NumericValidator *nv = new NumericValidator(le);
            nv->setBottom(sl.at(0));
            nv->setTop(sl.at(1));
            le->setValidator(nv);
				le->setToolTip(aValues);
         }
      }
		le->setText(aCurrent);
      formList.append(le);
		gridLayout->addWidget(le, currRow, 1, 1, 1);
   }

	currRow++;
}

QString ParamDialog::getFieldValue(int aIndex)
{
	QString retStr("");

	if (aIndex >= 0 && aIndex < formList.size())
	{
		QWidget *w = formList.at(aIndex);
		QString cn = w->metaObject()->className();

		if (cn == "QLineEdit")
		{
			retStr = ((QLineEdit*)w)->text();
		}
		else if (cn == "QComboBox")
		{
			retStr = ((QComboBox*)w)->currentText();
		}
	}
	return retStr;
}

/*!
The user click on the Ok-button this checking method is called. Here 
we look to all LineEdit fields and check if the have acceptable input
If one have not such input the form is not closed.

\todo mark the wrong input in the grid layout
*/
void ParamDialog::on_pbOk_clicked()
{
	QLineEdit tmpLE;
	bool bOk = true;

	foreach (QWidget *w, formList)
	{
		QString cn = w->metaObject()->className();

		if (cn == "QLineEdit")
		{
			QLineEdit *le = (QLineEdit*)w;

			if ( !le->hasAcceptableInput() )
			{
				bOk = false;
				//QeWidget::setBackgroundColor(le, Qt::red);
			}
			else
			{
				//QeWidget::setBackgroundColor(le, Qt::transparent, false);
			}
		}
	}
	if (bOk) this->hide();
}

void ParamDialog::on_pbCancel_clicked()
{
	wasCanceled = true;
	this->hide();
}
