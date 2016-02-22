#ifndef QTCASIM_H
#define QTCASIM_H

#include <QMainWindow>
#include "ui_qtcasim.h"
#include "AutomataFactory.h"
#include "Automata.h"

class QtCasim : public QMainWindow
{
    Q_OBJECT

public:
    QtCasim(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QtCasim();
    void setVisualizationWidget(QWidget *aVis);
    void setAutomata(Automata *aAutomata);
    Automata* getAutomata() { return automata; }
    void loadAutomata(const QString &name );

private:
    Ui::QtCasimClass ui;
    QVBoxLayout *fieldWidgetLayout;
    QWidget *fieldWidget;
    Automata *automata;
    AutomataFactory factory;
    volatile bool bStopped;

    bool checkAutomata();

private slots:
    void on_actionQuit_triggered();
    void on_btnNext_clicked();
    void on_actionClear_triggered();
    void on_actionRandom_triggered();
    void on_actionSaveField_triggered();
    void on_actionLoadField_triggered();
    void on_btnStart_clicked();
    void on_btnStopp_clicked();
    void on_actionLoad_triggered();
    void on_actionTuring_triggered();
    void on_actionMouse_State_triggered();
};

#endif
