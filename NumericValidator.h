#ifndef NUMERICVALIDATOR_H
#define NUMERICVALIDATOR_H

#include <QValidator>

class NumericValidator : public QValidator
{
public:
    NumericValidator(QObject *parent);
    NumericValidator(int bottom, int top, QObject *parent);
    ~NumericValidator();

    virtual QValidator::State validate(QString & input, int & pos) const ;
    virtual void fixup(QString& input) const;

    void setBottom(int bottom) { min = bottom; }
    void setTop(int top) { max = top; }

    void setBottom(QString bottom);
    void setTop(QString top);

private:
   int convertToInt(bool *bOk, QString str) const;
   int min;
   int max;
};

#endif // NUMERICVALIDATOR_H
