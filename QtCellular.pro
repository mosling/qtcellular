# -------------------------------------------------
# Project created by QtCreator 2011-06-06T12:43:55
# -------------------------------------------------
TARGET			= QtCellular
TEMPLATE		= app
TRANSLATIONS            = qtcellular_en.ts qtcellular_de.ts

QT			+= core gui widgets xml
CONFIG 			+= lex yacc
TRANSLATIONS            += qtcellular_en.ts qtcellular_de.ts
CODECFORTR		= UTF-8

DEFINES			+= YY_NEVER_INTERACTIVE
DEFINES			+= YY_VERBOSE
DEFINES			+= YY_NO_UNPUT

LEXSOURCES		+= casimScanner.l
YACCSOURCES		+= casimParser.y

QMAKE_LEX 		= flex
QMAKE_YACC 		= bison
QMAKE_LEXFLAGS	= -i
win32:QMAKE_YACCFLAGS	= -d -o y.tab.c
unix:QMAKE_YACCFLAGS	= -d
QMAKE_MOVE		= mv
QMAKE_DEL_FILE          = rm -f
QMAKE_CFLAGS_WARN_ON    = -Wall -pedantic -ansi

SOURCES += main.cpp \
    Queue.cpp \
    Expression.cpp \
    AutomataFactory.cpp \
    GridWidget.cpp \
    Grid.cpp \
    StateList.cpp \
    Rule.cpp \
    FieldAlgorithms.cpp \
    Ca.cpp \
    Automata.cpp \
    CellularException.cpp \
    QtCasim.cpp \
    Random.cpp \
    RuleSet.cpp \
    Turing.cpp \
    TuringRule.cpp \
    ParamDialog.cpp \
    NumericValidator.cpp

HEADERS += Queue.h \
    Expression.h \
    AutomataFactory.h \
    GridWidget.h \
    Field.h \
    Grid.h \
    StateList.h \
    Rule.h \
    FieldAlgorithms.h \
    Ca.h \
    Automata.h \
    CellularException.h \
    QtCasim.h \
    Random.h \
    RuleSet.h \
    Turing.h \
    TuringRule.h \
    ParamDialog.h \
    NumericValidator.h \
    Field.h \
    casimParser.y \
    casimScanner.l

FORMS += qtcasim.ui \
    ParamDialogWindow.ui

OTHER_FILES += automata/traffic.ca \
    automata/srdp.ca \
    automata/macaroni.ca \
    automata/life.ca \
    automata/lab2.ca \
    automata/gas.ca \
    qtcellular_en.ts \
    qtcellular_de.ts \
    Doxyfile \
    automata/life.xml

RESOURCES += \
    qtcellular.qrc

DISTFILES += \
    automata/lab2.xml
