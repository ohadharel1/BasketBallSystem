#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T16:15:20
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets



TARGET = BasketBallSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    fileexplorer.cpp \
    mainwindowlayout.cpp \
    guiformmanager.cpp \
    player.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    fileexplorer.h \
    mainwindowlayout.h \
    guiformmanager.h \
    player.h

FORMS    += mainwindow.ui \
    fileexplorer.ui \
    mainwindowlayout.ui \
    player.ui

RESOURCES += \
    resources.qrc
