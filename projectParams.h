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
 *                  <argument ID="0" name="nodeID" type="uint16" />
 *    ...
 *    -- value of the structure is --
 *    attributeName="ID" attributeValue="0"
 *    attributeName="name" attributeValue="nodeID"
 *    attributeName="type" attributeValue="uint16"
 *
 **/
// TODO: rename?
typedef QMap<QString, QString> AttrInfo;
// typedef AttrInfo PluginInfo;
typedef AttrInfo EventInfo;
typedef AttrInfo LogFileInfo;
typedef AttrInfo EventArgument;

class ProjectInfo
{
    // Q_OBJECT
public:
    // name of the author of the project
    QString projectAutor;
    // title of the project
    QString projectTitle;
    // comment of the project
    QString projectComment;

    QStringList keywords;

    // saves count
    int revision;

    // last modified in unix-time
    quint64 modified;
};    

// TODO: do this
// class PluginParams
// {
//     // Q_OBJECT
// public:
//     // plugin info
//     PluginInfo pluginInfo;
//     // plugin data
//     // TODO: check how it works
//     QVariantMap params;
// };


/**
 *
 * Template EventParams
 *    <event ID="?1" name="?2" group="?3">
 *      <argument ID="?4" name="?5" type="?6" />
 *      ...
 *    </event>
 *
 *    Both ID type is uint8
 *    TODO: write about other types, about type attr values, about numbering of events
 *
 **/
class EventParams
{
public:
    // event info
    EventInfo eventInfo;
    // event arguments
    QList<EventArgument> arguments;
};

/**
 *
 * Events agregator
 * Contains lists of system and users events
 *
 **/
class Events
{
public:
    // params
    QList<EventParams> systemEvents;
    QList<EventParams> userEvents;
};

/**
 *
 * Project params agregator
 * Contains project information, events and log files
 *
 **/
class ProjectParams 
{
public:
    QString version;
    ProjectInfo projectInfo;
    // QList<PluginParams> pluginsParams;
    Events events;
    QList<LogFileInfo> logFiles;
};

#endif // PROJECTPARAMS_H