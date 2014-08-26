#-------------------------------------------------
#
# Copyright (c) 2014 Ralph Janke. All rights reserved.
#
# Author: Ralph Janke virtnosis@jankeconsulting.ca
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_hypervisortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../../src/hypervisor.h

SOURCES += tst_hypervisortest.cpp
SOURCES += ../../src/hypervisor.cpp



DEFINES += SRCDIR=\\\"$$PWD/\\\"
