/**
 *
 * File: projectParams.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef PROJECTPARAMS_H
#define PROJECTPARAMS_H

#include <QtCore>

/**
 *
 * Template QMap<attributeName, attributeValue>
 *    For example:
 *    --  project.xml  --
 *    ...
 *          <plugin ID="0" namespace="env/asd" name="1" version="2" />
 *    ...
 *    -- value of the structure is --
 *    attributeName="ID" attributeValue="0"
 *    attributeName="namespace" attributeValue="env/asd"
 *    attributeName="name" attributeValue="1"
 *    attributeName="version" attributeValue="2"
 *
 **/
// TODO: rename to GeneralInfo?
typedef QMap<QString, QString> Info;
typedef Info PluginInfo;
typedef Info EventInfo;
typedef Info LogFileInfo;
typedef Info EventArgument;

class ProjectInfo
{
    // Q_OBJECT
public:
    // name of the author of the project
    QString projectAutor;
    // name of the project
    QString projectName;
    // comment of the project
    QString projectComment;
    // used plugins
    QList<PluginInfo> usedPlugins;
};    

class PluginParams
{
    // Q_OBJECT
public:
    // plugin info
    PluginInfo pluginInfo;
    // plugin data
    // TODO: check how it works
    QVariantMap params;
};

class EventParams
{
    // Q_OBJECT
public:
    // EventParams(){};
    // event info
    EventInfo eventInfo;
    // event arguments
    QList<EventArgument> arguments;
};

class Events
{
    // Q_OBJECT

public:
    // params
    QList<EventParams> pluginsEvents;
    QList<EventParams> userEvents;
};

// class LogFile
// {
//     Q_OBJECT
// public:
//     LogFileInfo info;
// };

// agregator
// class ProjectParams : public QObject 
class ProjectParams 
{
    // Q_OBJECT
public:
    QString version;
    ProjectInfo projectInfo;
    QList<PluginParams> pluginsParams;
    Events events;
    QList<LogFileInfo> logFiles;
// private:
    // Q_DISABLE_COPY(ProjectParams)
};

#endif // PROJECTPARAMS_H
