#-------------------------------------------------
#
# Project created by EmacsCreator 2012-2-14T15:21:24
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = projectData
DESTDIR = ../../bin/
TEMPLATE = lib

CONFIG += release
DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

DEFINES += "PROJECTDATA_VERSION=\'\"0.6.0\"\'" PROJECTDATA_PACK

SOURCES += projectData.cpp

HEADERS += projectData.h projectParams.h
