#-------------------------------------------------
#
# Copyright (c) 2014 Ralph Janke. All rights reserved.
#
# Author: Ralph Janke virtnosis@jankeconsulting.ca
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = subdirs
SUBDIRS += src \ 
    test/Hypervisor

CONFIG(release, debug|release) {
  DEFINES -= DEBUG
}

CONFIG(debug, debug|release) {
  DEFINES += DEBUG
}

CONFIG(debug, debug|release):message(Project is built in DEBUG mode.)
CONFIG(release, debug|release):message(Project is built in RELEASE mode.)

