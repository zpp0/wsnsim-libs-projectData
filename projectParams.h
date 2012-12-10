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
typedef QMap<QString, QString> AttrInfo;
typedef AttrInfo EventInfo;
typedef AttrInfo LogFileInfo;
typedef AttrInfo IndexFileInfo;
typedef AttrInfo EventArgument;
typedef AttrInfo ModuleInfo;

struct VisualizationInfo
{
    QString columnsSettings;
    QString formatsSettings;
};

struct ProjectInfo
{
    // name of the author of the project
    QString projectAutor;
    // title of the project
    QString projectTitle;
    // comment of the project
    QString projectComment;

    QStringList keywords;

    // saves count
    int revision;

    // last modified
    QString modified;
};

// Единицы измерений времени работы симулятора
typedef enum timeUnits {
    us, ms, s, m, h, d, w, months, years
} TimeUnits;

struct NodesData
{
    quint16 moduleID;
    QString nodeType;
    quint16 nodesNumber;
};

struct SimulatorParams
{
    // max simulation time
    quint64 maxTime;
    // max simulation time units
    TimeUnits timeUnits;
    // log file
    QString logFile;
};

struct ModuleParam
{
    // param name
    QString name;
    // param type
    QString type;
    // param value
    QVariant value;
};

struct ModuleDependence
{
    QString name;
    QString type;
    QString hasFunctions;
    QMap<QString, QList<EventArgument> > events;
    quint16 moduleID;
};

struct ModuleData
{
    // moduleInfo contains name, implementation's lang
    ModuleInfo moduleInfo;
    QString fileName;
    QList<ModuleParam> params;
    QList<ModuleDependence> dependencies;
};

struct NodeTypeData
{
    QString name;
    QList<quint16> hardwareModules;
    QList<quint16> softwareModules;
};

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
struct EventParams
{
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
struct Events
{
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
struct ProjectParams
{
    QString uuid;
    QString version;
    ProjectInfo projectInfo;
    SimulatorParams simulatorParams;
    QList<NodeTypeData> nodeTypes;
    // nodes numbers
    QList<NodesData> nodes;
    QList<ModuleData> modules;
    Events events;
    QList<LogFileInfo> logFiles;
    IndexFileInfo indexFileInfo;
    VisualizationInfo visualizationInfo;
};

#endif // PROJECTPARAMS_H
