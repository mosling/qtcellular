#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include <QDialog>
#include <QList>
#include <QLineEdit>
#include <QGridLayout>
#include "ui_paramdialogwindow.h"

class ParamDialog : public QDialog
{
    Q_OBJECT

public:
    ParamDialog(QWidget *parent = 0);
    ~ParamDialog();
    void addInputField(QString label, QString aValues, QString aCurrent = "");
	 QString getFieldValue(int aIndex);
	 bool inputCanceled() { return wasCanceled; }

private:
    Ui::ParamDialogWindowClass *ui;
    QList<QWidget*> formList;
	 bool wasCanceled;
	 QGridLayout *gridLayout;
	 unsigned int currRow;

private slots:
	void on_pbOk_clicked();
	void on_pbCancel_clicked();
};

#endif // PARAMDIALOG_H
