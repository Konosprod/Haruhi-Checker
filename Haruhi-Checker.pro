#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T06:56:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Haruhi-Checker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    adddialog.cpp \
    resultwindows.cpp \
    preferencedialog.cpp

HEADERS  += mainwindow.h \
    adddialog.h \
    resultwindows.h \
    preferencedialog.h

FORMS    += mainwindow.ui \
    adddialog.ui \
    resultwindows.ui \
    preferencedialog.ui

RESOURCES += \
    rc.qrc
