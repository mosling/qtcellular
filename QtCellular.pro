#-------------------------------------------------
#
# Project created by QtCreator 2011-06-06T12:43:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = QtCellular
TEMPLATE = app

DEFINES += YY_NEVER_INTERACTIVE
DEFINES += YY_VERBOSE
DEFINES += YY_NO_UNPUT

LEXSOURCES += casimScanner.l
YACCSOURCES += casimParser.y

CONFIG += lex yacc
QMAKE_LEX      = flex
QMAKE_YACC     = bison
QMAKE_LEXFLAGS = -i
QMAKE_YACCFLAGS= -d -o y.tab.c

QMAKE_MOVE     = mv
QMAKE_DEL_FILE = rm -f
QMAKE_CFLAGS_WARN_ON	= -Wall -pedantic -ansi

SOURCES += main.cpp\
    CQueue.cpp \
    CExpression.cpp \
    AutomataFactory.cpp \
    GridWidget.cpp \
    Grid.cpp \
    StateList.cpp \
    Rule.cpp \
    FieldAlgorithms.cpp \
    Ca.cpp \
    Automata.cpp \
    StopWatch.cpp \
    CellularException.cpp \
    QtCasim.cpp \
    Random.cpp \
    RuleSet.cpp \
    Turing.cpp \
    TuringRule.cpp \
    ParamDialog.cpp \
    NumericValidator.cpp

HEADERS  += \
    CQueue.h \
    CExpression.h \
    AutomataFactory.h \
    GridWidget.h \
    Field.h \
    Grid.h \
    StateList.h \
    Rule.h \
    FieldAlgorithms.h \
    Ca.h \
    Automata.h \
    global.h \
    StopWatch.h \
    CellularException.h \
    QtCasim.h \
    Random.h \
    RuleSet.h \
    Turing.h \
    TuringRule.h \
    ParamDialog.h \
    NumericValidator.h \
    Field.h

FORMS    += \
    qtcasim.ui \
    ParamDialogWindow.ui

OTHER_FILES += \
    ../QtCellular-build-desktop/life.ca \
    ../QtCellular-build-desktop/macaroni.ca \
    ../QtCellular-build-desktop/srdp.ca \
    ../QtCellular-build-desktop/traffic.ca \
    ../QtCellular-build-desktop/lab2.ca \
    ../QtCellular-build-desktop/life.xml \
    ../QtCellular-build-desktop/gas.ca \
    automata/traffic.ca \
    automata/srdp.ca \
    automata/macaroni.ca \
    automata/life.ca \
    automata/lab2.ca \
    automata/gas.ca











