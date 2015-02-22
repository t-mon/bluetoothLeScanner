#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T18:36:07
#
#-------------------------------------------------

QT       += core bluetooth network

QT       -= gui

TARGET = bluetoothLeScanner
CONFIG   += console
CONFIG   -= app_bundle

target.path = /usr/bin
INSTALLS += target

TEMPLATE = app

SOURCES += main.cpp \
    core.cpp \
    bluetoothscanner.cpp \
    device.cpp \
    characteristicinfo.cpp \
    serviceinfo.cpp

HEADERS += \
    core.h \
    bluetoothscanner.h \
    device.h \
    characteristicinfo.h \
    serviceinfo.h
