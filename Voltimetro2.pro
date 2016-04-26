#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T12:21:06
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Voltimetro2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    settingdialog.ui

RESOURCES += \
    terminal.qrc
