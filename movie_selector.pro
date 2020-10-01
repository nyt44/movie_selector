#-------------------------------------------------
#
# Project created by QtCreator 2017-10-17T20:41:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = movie_selector
TEMPLATE = app

win32:QMAKE_CXXFLAGS += /std:c++17
unix:QMAKE_CXXFLAGS += -std=c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH = include

#unix:INCLUDEPATH += /usr/include/glib-2.0 \
#                    /usr/lib/x86_64-linux-gnu/glib-2.0/include/

unix:LIBS += -lstdc++fs

SOURCES += \
    source/configuration.cpp \
    source/episode_selector_wizard_page.cpp \
    source/main.cpp \
    source/map_collector.cpp \
    source/series_selector_wizard_page.cpp \
    source/wizard.cpp

#win32:SOURCES += source/windows_episode_runner.cpp
unix:SOURCES += source/unix_episode_runner.cpp

HEADERS += \
    include/configuration.hpp \
    include/episode_runner.hpp \
    include/episode_selector_wizard_page.hpp \
    include/map_collector.hpp \
    include/series_selector_wizard_page.hpp \
    include/wizard.hpp
