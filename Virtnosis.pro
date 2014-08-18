#-------------------------------------------------
#
# Project created by QtCreator 2014-08-16T15:57:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = subdirs
SUBDIRS += src 

CONFIG(release, debug|release) {
  DEFINES -= DEBUG
}

CONFIG(debug, debug|release) {
  DEFINES += DEBUG
}

CONFIG(debug, debug|release):message(Project is built in DEBUG mode.)
CONFIG(release, debug|release):message(Project is built in RELEASE mode.)

