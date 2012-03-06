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

    ProjectParams load(QString& projectFileName, QString* errorMessage, bool loadPluginParams);
    QFile::FileError create(QString& projectFileName, ProjectParams params);

    QString version;

private:

    Info loadInfo(QDomNode dn_node);
    
    ProjectInfo loadProjectInfo(QDomNode dn_node);

    PluginParams loadPluginParams(QDomNode dn_node);
    QList<PluginParams> loadPluginsParams(QDomNode dn_node);
    
    EventParams loadEventParams(QDomNode dn_node);
    Events loadEvents(QDomNode dn_node);

    QList<LogFileInfo> loadLogFiles(QDomNode dn_node);

};

#endif // PROJECTDATA_H
