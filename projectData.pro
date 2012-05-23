include(../libs.pri)

QT       -= gui
QT       += xml

DEFINES += "PROJECTDATA_VERSION=\'\"0.6.0\"\'" PROJECTDATA_PACK

SOURCES += projectData.cpp
HEADERS += projectData.h projectParams.h
