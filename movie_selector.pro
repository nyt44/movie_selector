#-------------------------------------------------
#
# Project created by QtCreator 2017-10-17T20:41:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = movie_selector
TEMPLATE = app
QMAKE_CXXFLAGS += /std:c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH = gui/include/ \
              msw/include

unix:INCLUDEPATH += /usr/include/glib-2.0 \
                    /usr/lib/x86_64-linux-gnu/glib-2.0/include/

unix:LIBS += -lglib-2.0 -lstdc++fs

SOURCES += \
    msw/source/main.cpp \
    gui/source/mainwizardpage.cpp \
    gui/source/movieselectorwizard.cpp \
    gui/source/seriesselectorwizardpage.cpp \
    msw/source/singleton.cpp \
    msw/source/threadmgr.cpp \
    msw/source/seriesdatakeeper.cpp

win32:SOURCES += msw/source/windowsrunner.cpp
unix:SOURCES += msw/source/unixrunner.cpp

HEADERS += \
    gui/include/mainwizardpage.h \
    gui/include/movieselectorwizard.h \
    gui/include/seriesselectorwizardpage.h \
    msw/include/singleton.h \
    msw/include/threadmgr.h \
    msw/include/seriesdatakeeper.h \
    msw/include/runnerinterface.h

win32:HEADERS += msw/include/windowsrunner.h
unix:HEADERS += msw/include/unixrunner.h
