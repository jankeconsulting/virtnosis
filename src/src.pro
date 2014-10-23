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
    hypervisor.cpp \
    hypervisordialog.cpp \
    domainviewitemdelegate.cpp \
    domainviewmodel.cpp \
    domainitem.cpp \
    hypervisoritem.cpp \
    domain.cpp \
    aboutdialog.cpp \
    licensedialog.cpp \
    settingsdialog.cpp

HEADERS  += virtnosiswindow.h \
    hypervisor.h \
    hypervisordialog.h \
    domainviewitemdelegate.h \
    domainviewmodel.h \
    domainitem.h \
    hypervisoritem.h \
    domain.h \
    aboutdialog.h \
    licensedialog.h \
    settingsdialog.h

FORMS    += virtnosiswindow.ui \
    hypervisordialog.ui \
    aboutdialog.ui \
    licensedialog.ui \
    settingsdialog.ui

unix|win32: LIBS += -lvirt

RESOURCES += \
    virtnosis.qrc

virtnosis_install.files = virtnosis
virtnosis_install.path = /usr/bin/
INSTALLS += virtnosis_install

CONFIG(release, debug|release) {
  DEFINES -= DEBUG
}

CONFIG(debug, debug|release) {
  DEFINES += DEBUG
}
