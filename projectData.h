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

class ProjectData : public QObject
{
    Q_OBJECT
public:

    // конструктор
    ProjectData();

    ProjectParams load(QString& projectFileName, QString* errorMessage);
    void save(QString& projectFileName, QString* errorMessage, ProjectParams params);

    QString version;

private:

    AttrInfo loadInfo(QDomNode dn_node);

    ProjectInfo loadProjectInfo(QDomNode dn_node);
    SimulatorParams loadSimulatorParams(QDomNode dn_node);
    EventParams loadEventParams(QDomNode dn_node);
    QList<ModuleData> loadModules(QDomNode dn_node);
    Events loadEvents(QDomNode dn_node);
    QList<LogFileInfo> loadLogFiles(QDomNode dn_node);

    ModuleData loadModule(QDomNode dn_node);
    QList<ModuleParam> loadModuleParams(QDomNode dn_node);

    // void saveInfo(QDomElement* parent, AttrInfo info);
    void saveProjectInfo(QDomDocument* result, QDomElement* parent, ProjectInfo info);
    void saveModules(QDomDocument* result, QDomElement* parent, QList<ModuleData> modules);
    void saveModulesParams(QDomDocument* result, QDomElement* parent, QList<ModuleParam> modulesParams);
    void saveSimulatorParams(QDomDocument* result, QDomElement* parent, SimulatorParams simParams);
    void saveEventParams(QDomDocument* result, QDomElement* parent, EventParams eventParams);
    void saveEvents(QDomDocument* result, QDomElement* parent, Events events);
    void saveLogFiles(QDomDocument* result, QDomElement* parent, QList<LogFileInfo> logFilesInfo);

    void createXml(QDomDocument* result, QDomElement* parent, QString XNodeName, QString XNodeValue);
    void createXml(QDomDocument* result, QDomElement* parent, QString XNodeName, QString XNodeValue, QMap<QString, QString> attrs);
};

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

extern "C" MY_EXPORT ProjectParams load(QString& projectFileName, QString* errorMessage);
extern "C" MY_EXPORT void save(QString& projectFileName, QString* errorMessage, ProjectParams params);

#endif // PROJECTDATA_H
