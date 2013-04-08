#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T12:13:11
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = cqueue_test
CONFIG   += console
CONFIG   -= app_bundle
INCLUDEPATH += ..

TEMPLATE = app


SOURCES += \
	..\Queue.cpp

SOURCES += \
    Queue_Test.cpp \
    main_test.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    Queue_Test.h
