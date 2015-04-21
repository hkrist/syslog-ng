#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T20:30:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogFlowGUI
TEMPLATE = app


SOURCES += main.cpp \
    dialog.cpp \
    visualizationscene.cpp \
    visualelement.cpp \
    logparserthread.cpp

HEADERS  += \
    dialog.h \
    visualizationscene.h \
    visualelement.h \
    logparserthread.h

FORMS    += \
    dialog.ui
