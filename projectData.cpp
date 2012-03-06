/**
 *
 * File: projectData.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include <QtXml>
#include <QDomElement>
#include <QDomDocument>
#include <QDomText>

#ifndef PROJECTDATA_VERSION
#warning : ProjectData lib version undefined!
#define PROJECTDATA_VERSION ""
#endif

#include "projectData.h"

ProjectData::ProjectData()
{
    version = PROJECTDATA_VERSION;
}

ProjectParams ProjectData::load(QString& projectFileName, QString* errorMessage, bool loadPluginParams) {

    ProjectParams projectParams;

    QFile file(projectFileName);

#ifdef PROJECTDATA_PACK
    // TODO:
#endif

    QDomDocument dd_doc;

    int errorLine;
    int errorColumn;

    if (!file.open(QFile::ReadOnly)) {

#ifdef PROJECTDATA_DEBUG
        // TODO: generate more readable message
        qDebug("Error open file: %s", qPrintable(file.error()));
        *errorMessage = QString::number(file.error);
#endif

        return projectParams;        
    }
        
    QString content = file.readAll();
    file.close();

    // переносим текстовую информацию из файла в объекты XML
    if (!dd_doc.setContent(content, true, errorMessage, &errorLine, &errorColumn)) {

#ifdef PROJECTDATA_DEBUG
        qDebug("Error in xml structure file: %s\n line %x\n column %x ", qPrintable(*errorMessage), errorLine, errorColumn);
#endif

        return projectParams;
    }

    // переходим к данным XML
    QDomElement de_root = dd_doc.documentElement();

    projectParams.version = de_root.text();

    QDomNode dn_node = de_root.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_node.isNull()) {

        // раздел projectInfo
        if (dn_node.nodeName() == "projectInfo")
            projectParams.projectInfo = loadProjectInfo(dn_node);

        if (dn_node.nodeName() == "pluginsParams")
            if (loadPluginParams)
                projectParams.pluginsParams = loadPluginsParams(dn_node);

        if (dn_node.nodeName() == "events")
            projectParams.events = loadEvents(dn_node);

        if (dn_node.nodeName() == "logFiles")
            projectParams.logFiles = loadLogFiles(dn_node);

        // переходим к следующему узлу XML
        dn_node = dn_node.nextSibling();
    }

    // возвращаем результат
    return projectParams;
}

ProjectInfo ProjectData::loadProjectInfo(QDomNode dn_node)
{
    ProjectInfo projectInfo;
    
    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull ()) {

        // узел - version
        if (dn_nextNode.nodeName () == "author")
            projectInfo.projectAutor = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName () == "name")
            projectInfo.projectName = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "comment")
            projectInfo.projectComment = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "usedPlugins") {
            QDomNode dn_usedPlugins = dn_nextNode.firstChild();

            // распознаем дочерние узлы, пока они не закончатся
            while (!dn_usedPlugins.isNull()) {

                if (dn_usedPlugins.nodeName() == "plugin")
                    projectInfo.usedPlugins += loadInfo(dn_usedPlugins);

                dn_usedPlugins = dn_usedPlugins.nextSibling();
            }
        }
        
        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

#ifdef PROJECTDATA_DEBUG
    qDebug("projectAutor is %s", qPrintable(projectInfo.projectAutor)); 
    qDebug("projectName is %s", qPrintable(projectInfo.projectName)); 
    qDebug("projectComment is %s", qPrintable(projectInfo.projectComment)); 
    
    foreach (PluginInfo pluginInfo, projectInfo.usedPlugins) {
        foreach (QString key, pluginInfo.keys()) {
            qDebug("%s is %s", qPrintable(key), qPrintable(pluginInfo[key]));
        }
    }
#endif

    // возвращаем результат
    return projectInfo;
}

PluginParams ProjectData::loadPluginParams(QDomNode dn_node) {
    PluginParams pluginParams;
    pluginParams.pluginInfo = loadInfo(dn_node);
    // TODO: write it
    return pluginParams;
}

QList<PluginParams> ProjectData::loadPluginsParams(QDomNode dn_node) 
{
    QList<PluginParams> pluginParams;
    // TODO: write it
    return pluginParams;
    
}

EventParams ProjectData::loadEventParams(QDomNode dn_node)
{
    EventParams eventParams;
    eventParams.eventInfo = loadInfo(dn_node);

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull ()) {

        if (dn_nextNode.nodeName() == "argument")
            eventParams.arguments += loadInfo(dn_nextNode);
        
        dn_nextNode = dn_nextNode.nextSibling();
    }

    return eventParams;
}

Events ProjectData::loadEvents(QDomNode dn_node)
{
    Events events;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull ()) {

        // TODO: make it easy
        if (dn_nextNode.nodeName() == "pluginEvents") {
            QDomNode dn_pluginEvents = dn_nextNode.firstChild();

            // распознаем дочерние узлы, пока они не закончатся
            while (!dn_pluginEvents.isNull()) {

                if (dn_pluginEvents.nodeName() == "event")
                    events.pluginsEvents += loadEventParams(dn_pluginEvents);

                dn_pluginEvents = dn_pluginEvents.nextSibling();
            }
        }

        if (dn_nextNode.nodeName() == "usedPlugins") {
            QDomNode dn_userEvents = dn_nextNode.firstChild();

            // распознаем дочерние узлы, пока они не закончатся
            while (!dn_userEvents.isNull()) {

                if (dn_userEvents.nodeName() == "event")
                    events.userEvents += loadEventParams(dn_userEvents);

                dn_userEvents = dn_userEvents.nextSibling();
            }
        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

#ifdef PROJECTDATA_DEBUG
    // TODO: remove this copy-paste
    foreach (EventParams eventParams, events.pluginsEvents) {
        foreach (QString key, eventParams.eventInfo.keys()) {
            qDebug("%s is %s", qPrintable(key), qPrintable(eventParams.eventInfo[key]));
        }
        
        foreach (EventArgument eventArgument, eventParams.arguments) {
            foreach (QString key, eventArgument.keys()) {
                qDebug("%s is %s", qPrintable(key), qPrintable(eventArgument[key]));
            }
        }
    }
    foreach (EventParams eventParams, events.userEvents) {
        foreach (QString key, eventParams.eventInfo.keys()) {
            qDebug("%s is %s", qPrintable(key), qPrintable(eventParams.eventInfo[key]));
        }
        
        foreach (EventArgument eventArgument, eventParams.arguments) {
            foreach (QString key, eventArgument.keys()) {
                qDebug("%s is %s", qPrintable(key), qPrintable(eventArgument[key]));
            }
        }
    }
#endif

    return events;
}

QList<LogFileInfo> ProjectData::loadLogFiles(QDomNode dn_node)
{
    QList<LogFileInfo> logFiles;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        // узел - version
        if (dn_nextNode.nodeName() == "logFile")
            logFiles += loadInfo(dn_nextNode);
        
        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

#ifdef PROJECTDATA_DEBUG
    foreach (LogFileInfo logFileInfo, logFiles) {
        foreach (QString key, logFileInfo.keys()) {
            qDebug("%s is %s", qPrintable(key), qPrintable(logFileInfo[key]));
        }
    }
#endif


    // возвращаем результат
    return logFiles;
}

Info ProjectData::loadInfo(QDomNode dn_node)
{
    Info info;

    if (dn_node.hasAttributes()) {
        
        QDomNamedNodeMap attributes = dn_node.attributes();

        for (uint index = 0; index < attributes.length(); index++) {
            QDomAttr attr = attributes.item(index).toAttr();
            info[attr.name()] = attr.value();
        }
    }

    return info;
}
