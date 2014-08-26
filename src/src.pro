#-------------------------------------------------
#
# Copyright (c) 2014 Ralph Janke. All rights reserved.
#
# Author: Ralph Janke virtnosis@jankeconsulting.ca
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = virtnosis
TEMPLATE = app

SOURCES += main.cpp\
        virtnosiswindow.cpp \
    hypervisor.cpp

HEADERS  += virtnosiswindow.h \
    hypervisor.h

FORMS    += virtnosiswindow.ui

unix|win32: LIBS += -lvirt
