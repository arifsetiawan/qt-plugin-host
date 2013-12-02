#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T22:13:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PluginHost
TEMPLATE = app

DESTDIR = ../Output

SOURCES += main.cpp\
        mainwindow.cpp \
    newdialog.cpp \
    uploadsettingsdialog.cpp

HEADERS  += mainwindow.h \
    newdialog.h \
    uploadsettingsdialog.h

FORMS    += mainwindow.ui \
    newdialog.ui \
    uploadsettings.ui \
    downloadsettings.ui
