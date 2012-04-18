/**
 *
 * File: projectData.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QtCore>
#include <QtXml>

#include "projectParams.h"

/**
 *
 * Класс для работы с файлом проекта симулятора
 *
 **/

class ProjectData : public QObject {
    Q_OBJECT
public:

    // конструктор
    ProjectData();

    ProjectParams load(QString& projectFileName, QString* errorMessage);

    QString version;

    // TODO: do it
    // QFile::FileError create(QString& projectFileName, ProjectParams params);

private:

    AttrInfo loadInfo(QDomNode dn_node);
    
    ProjectInfo loadProjectInfo(QDomNode dn_node);

    QList<ModuleParams> loadModulesParams(QDomNode dn_node);
    
    SimulatorParams loadSimulatorParams(QDomNode dn_node);

    EventParams loadEventParams(QDomNode dn_node);
    Events loadEvents(QDomNode dn_node);

    QList<LogFileInfo> loadLogFiles(QDomNode dn_node);

};

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

extern "C" MY_EXPORT ProjectParams load(QString& projectFileName, QString* errorMessage);

#endif // PROJECTDATA_H
