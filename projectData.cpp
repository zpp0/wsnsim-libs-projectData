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

ProjectParams ProjectData::load(QString& projectFileName, QString* errorMessage)
{

    ProjectParams projectParams;

    QFile file(projectFileName);

#ifdef PROJECTDATA_PACK
    // TODO:
#endif

    QDomDocument dd_doc;

    int errorLine;
    int errorColumn;

    if (!file.open(QFile::ReadOnly)) {

        // TODO: generate more readable message
        qDebug("Error open file: %s", qPrintable(file.error()));
        *errorMessage = QString::number(file.error());

        return projectParams;
    }

    QString content = file.readAll();
    file.close();

    // переносим текстовую информацию из файла в объекты XML
    if (!dd_doc.setContent(content, true, errorMessage, &errorLine, &errorColumn)) {

        qDebug("Error in xml structure file: %s\n line %x\n column %x ",
               qPrintable(*errorMessage), errorLine, errorColumn);

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

        if (dn_node.nodeName() == "events")
            projectParams.events = loadEvents(dn_node);

        if (dn_node.nodeName() == "modulesParams")
            projectParams.modulesParams = loadModulesParams(dn_node);

        if (dn_node.nodeName() == "simulatorParams")
            projectParams.simulatorParams = loadSimulatorParams(dn_node);

        if (dn_node.nodeName() == "logFiles")
            projectParams.logFiles = loadLogFiles(dn_node);

        // переходим к следующему узлу XML
        dn_node = dn_node.nextSibling();
    }

    qDebug("end");

    // возвращаем результат
    return projectParams;
}

ProjectInfo ProjectData::loadProjectInfo(QDomNode dn_node)
{
    ProjectInfo projectInfo;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        if (dn_nextNode.nodeName() == "author")
            projectInfo.projectAutor = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "title")
            projectInfo.projectTitle = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "comment")
            projectInfo.projectComment = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "keywords")
            projectInfo.keywords = dn_nextNode.toElement().text().split(" ");

        if (dn_nextNode.nodeName() == "revision")
            projectInfo.revision = dn_nextNode.toElement().text().toInt();

        if (dn_nextNode.nodeName() == "modified")
            projectInfo.modified = dn_nextNode.toElement().text().toInt();

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- projectInfo --");
    qDebug("projectAutor is %s", qPrintable(projectInfo.projectAutor));
    qDebug("projectTitle is %s", qPrintable(projectInfo.projectTitle));
    qDebug("projectComment is %s", qPrintable(projectInfo.projectComment));
    foreach(QString keyword, projectInfo.keywords)
        qDebug("keyword is %s", qPrintable(keyword));
    qDebug("revision number is %i", projectInfo.revision);
    qDebug("last modified at %llu", projectInfo.modified);

    // возвращаем результат
    return projectInfo;
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
        if (dn_nextNode.nodeName() == "systemEvents") {
            QDomNode dn_pluginEvents = dn_nextNode.firstChild();

            // распознаем дочерние узлы, пока они не закончатся
            while (!dn_pluginEvents.isNull()) {

                if (dn_pluginEvents.nodeName() == "event")
                    events.systemEvents += loadEventParams(dn_pluginEvents);

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

    // TODO: remove this copy-paste
    qDebug("-- system Events --");
    foreach (EventParams eventParams, events.systemEvents) {
        foreach (QString key, eventParams.eventInfo.keys()) {
            qDebug("%s is %s",
                   qPrintable(key), qPrintable(eventParams.eventInfo[key]));
        }

        foreach (EventArgument eventArgument, eventParams.arguments) {
            qDebug("-- argument");
            foreach (QString key, eventArgument.keys()) {
                qDebug("%s is %s",
                       qPrintable(key), qPrintable(eventArgument[key]));
            }
        }
        qDebug("----");
    }
    qDebug("-- user Events --");
    foreach (EventParams eventParams, events.userEvents) {
        foreach (QString key, eventParams.eventInfo.keys()) {
            qDebug("%s is %s",
                   qPrintable(key), qPrintable(eventParams.eventInfo[key]));
        }

        foreach (EventArgument eventArgument, eventParams.arguments) {
            qDebug("-- argument");
            foreach (QString key, eventArgument.keys()) {
                qDebug("%s is %s",
                       qPrintable(key), qPrintable(eventArgument[key]));
            }
        }
        qDebug("----");
    }

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

    qDebug("-- log Files --");
    foreach (LogFileInfo logFileInfo, logFiles) {
        foreach (QString key, logFileInfo.keys()) {
            qDebug("%s is %s", qPrintable(key), qPrintable(logFileInfo[key]));
        }
        qDebug("----");
    }

    // возвращаем результат
    return logFiles;
}

AttrInfo ProjectData::loadInfo(QDomNode dn_node)
{
    AttrInfo info;

    if (dn_node.hasAttributes()) {

        QDomNamedNodeMap attributes = dn_node.attributes();

        for (uint index = 0; index < attributes.length(); index++) {
            QDomAttr attr = attributes.item(index).toAttr();
            info[attr.name()] = attr.value();
        }
    }

    return info;
}

QList<ModuleParams> ProjectData::loadModulesParams(QDomNode dn_node)
{
    QList<ModuleParams> params;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        // узел - version
        if (dn_nextNode.nodeName() == "module") {
            ModuleParams moduleParams;

            QDomNamedNodeMap attributes = dn_nextNode.attributes();

            // FIXME: do smt with it
            // for (uint index = 0; index < attributes.length(); index++) {
            QDomAttr attr = attributes.item(0).toAttr();
            // info[attr.name()] = attr.value();
            // }

            // WARNING: this will be changed!
            // and now this is not module name
            moduleParams.moduleName = attr.value();

            // get child nodes
            QDomNode dn_params = dn_nextNode.firstChild();

            // распознаем дочерние узлы, пока они не закончатся
            while (!dn_params.isNull()) {
                ModuleParam moduleParam;

                if (dn_params.nodeName() == "param") {
                    QDomNamedNodeMap attributes = dn_params.attributes();
                    for (uint index = 0; index < attributes.length(); index++) {
                        QDomAttr attr = attributes.item(index).toAttr();
                        if (attr.name() == "name")
                            moduleParam.name = attr.value();
                        if (attr.name() == "type")
                            moduleParam.type = attr.value();
                        // if (attr.name() == "value")
                            // moduleParam.value = attr.value();
                    }

                    moduleParam.value = dn_params.toElement().text();

                    moduleParams.params += moduleParam;

                }

                dn_params = dn_params.nextSibling();
            }

            params += moduleParams;
        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- modules params --");
    foreach (ModuleParams moduleParams, params) {
        qDebug("module: %s", qPrintable(moduleParams.moduleName));
        foreach (ModuleParam moduleParam, moduleParams.params) {
            qDebug("name: %s, type: %s, value: %s",
                   qPrintable(moduleParam.name),
                   qPrintable(moduleParam.type),
                   qPrintable(moduleParam.value));
        }
        qDebug("----");
    }

    return params;
}

SimulatorParams ProjectData::loadSimulatorParams(QDomNode dn_node)
{
    SimulatorParams simulatorParams;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        if (dn_nextNode.nodeName() == "maxTime")
            // FIXME: is it work with quint64?
            simulatorParams.maxTime = dn_nextNode.toElement().text().toULong();

        // WARNING: this is hack
        if (dn_nextNode.nodeName() == "logFile")
            simulatorParams.logFile = dn_nextNode.toElement().text();

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- simulator params --");
    qDebug("max time: %llu", simulatorParams.maxTime);
    qDebug("logFile: %s", qPrintable(simulatorParams.logFile));

    return simulatorParams;
}

extern "C" MY_EXPORT ProjectParams load(QString& projectFileName, QString* errorMessage)
{
    ProjectData projectData;
    return projectData.load(projectFileName, errorMessage);
}
