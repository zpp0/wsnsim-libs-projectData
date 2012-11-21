/**
 *
 * File: projectData_load.cpp
 * Description: methods of loading project data from XML
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "projectData.h"

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
    // qDebug("last modified at %llu", projectInfo.modified);

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

        if (dn_nextNode.nodeName() == "userEvents") {
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

QList<ModuleParam> ProjectData::loadModuleParams(QDomNode dn_node)
{
    QList<ModuleParam> moduleParams;

    // param node
    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        // loading module param
        if (dn_nextNode.nodeName() == "param") {
            ModuleParam param;

            // getting xml node attributes
            QMap<QString, QString> attrs = loadInfo(dn_nextNode);
            param.name = attrs["name"];
            param.type = attrs["type"];

            // getting value of param
            if (param.type == "table") {
                // TODO: implement this
            }
            if (param.type == "Probability distribution") {
                // TODO: implement this
            }
            // other types
            else
                param.value = dn_nextNode.toElement().text();

            // adding param to list
            moduleParams += param;
        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    return moduleParams;
}

ModuleData ProjectData::loadModule(QDomNode dn_node)
{
    ModuleData module;

    // getting moduleInfo from current XML node
    module.moduleInfo = loadInfo(dn_node);

    // child node
    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        // getting fileName
        if (dn_nextNode.nodeName() == "fileName")
            module.fileName = dn_nextNode.toElement().text();

        // getting params
        if (dn_nextNode.nodeName() == "params")
            module.params = loadModuleParams(dn_nextNode);

        // getting dependencies
        if (dn_nextNode.nodeName() == "dependencies") {
            QDomNode dn_dependNode = dn_nextNode.firstChild();

            // перебираем узлы, пока не закончатся
            while (!dn_dependNode.isNull()) {
                // module
                if (dn_dependNode.nodeName() == "module") {
                    ModuleDependence depend;
                    depend.moduleID = dn_dependNode.toElement().text().toInt();
                    QMap<QString, QString> info = loadInfo(dn_dependNode);
                    depend.name = info["name"];
                    depend.type = info["type"];

                    module.dependencies += depend;
                }

                // переходим к следующему узлу
                dn_dependNode = dn_dependNode.nextSibling();
            }

        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    return module;
}

QList<ModuleData> ProjectData::loadModules(QDomNode dn_node)
{
    QList<ModuleData> modules;

    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        // getting module
        if (dn_nextNode.nodeName() == "module")
            modules += loadModule(dn_nextNode);

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- modules --");
    foreach(ModuleData module, modules) {
        qDebug("-- module --");
        foreach(QString param, module.moduleInfo.keys())
            qDebug("param %s is %s", qPrintable(param), qPrintable(module.moduleInfo[param]));
        qDebug("fileName is %s", qPrintable(module.fileName));
        foreach(ModuleParam param, module.params)
            qDebug("param %s of type %s with value %s",
                   qPrintable(param.name), qPrintable(param.type), qPrintable(param.value.toString()));
        foreach(ModuleDependence depend, module.dependencies)
            qDebug("module depends on %u", depend.moduleID);
    }

    return modules;
}

QList<NodeTypeData> ProjectData::loadNodeTypes(QDomNode dn_node)
{
    QList<NodeTypeData> nodeTypes;
    QDomNode dn_nextNode = dn_node.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_nextNode.isNull()) {

        if (dn_nextNode.nodeName() == "nodeType") {
            NodeTypeData nodeType;
            QMap<QString, QString> attrs = loadInfo(dn_nextNode);

            nodeType.name = attrs["name"];

            QDomNode dn_nodeTypeNode = dn_nextNode.firstChild();

            // перебираем узлы, пока не закончатся
            while (!dn_nodeTypeNode.isNull()) {

                if (dn_nodeTypeNode.nodeName() == "hardware") {
                    QDomNode dn_hardwareNode = dn_nodeTypeNode.firstChild();

                    while (!dn_hardwareNode.isNull()) {
                        if (dn_hardwareNode.nodeName() == "module")
                            nodeType.hardwareModules += dn_nextNode.toElement().text().toUInt();

                        dn_hardwareNode = dn_hardwareNode.nextSibling();
                    }
                }

                if (dn_nodeTypeNode.nodeName() == "software") {
                    QDomNode dn_softwareNode = dn_nodeTypeNode.firstChild();

                    while (!dn_softwareNode.isNull()) {
                        if (dn_softwareNode.nodeName() == "module")
                            nodeType.softwareModules += dn_nextNode.toElement().text().toUInt();

                        dn_softwareNode = dn_softwareNode.nextSibling();
                    }
                }

                dn_nodeTypeNode = dn_nodeTypeNode.nextSibling();
            }

            nodeTypes += nodeType;

        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- node Types --");
    foreach(NodeTypeData nodeType, nodeTypes) {
        qDebug("-- node Type --");
        qDebug() << "name" << nodeType.name;
        foreach(quint16 moduleID, nodeType.hardwareModules)
            qDebug("hardware %ui", moduleID);
        foreach(quint16 moduleID, nodeType.softwareModules)
            qDebug("hardware %ui", moduleID);
    }

    return nodeTypes;
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

        if (dn_nextNode.nodeName() == "timeUnits")
            // FIXME: is it work with quint64?
            simulatorParams.timeUnits = TimeUnits(dn_nextNode.toElement().text().toInt());

        if (dn_nextNode.nodeName() == "logFile")
            simulatorParams.logFile = dn_nextNode.toElement().text();

        if (dn_nextNode.nodeName() == "nodesNumber") {
            QDomNode dn_node = dn_nextNode.firstChild();

            // перебираем узлы, пока не закончатся
            while (!dn_node.isNull()) {
                if (dn_node.nodeName() == "nodes") {
                    NodesData nodesData;
                    QMap<QString, QString> paramsAttrs = loadInfo(dn_node);

                    nodesData.moduleID = paramsAttrs["moduleID"].toInt();
                    nodesData.nodeType = paramsAttrs["nodeType"];
                    nodesData.nodesNumber = dn_node.toElement().text().toInt();

                    simulatorParams.nodes += nodesData;
                }

                dn_node = dn_node.nextSibling();
            }

        }

        // переходим к следующему узлу
        dn_nextNode = dn_nextNode.nextSibling();
    }

    qDebug("-- simulator params --");
    qDebug("max time: %llu", simulatorParams.maxTime);
    qDebug("logFile: %s", qPrintable(simulatorParams.logFile));

    return simulatorParams;
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

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("utf-8"));

    QString content = out.readAll();
    file.close();

    // переносим текстовую информацию из файла в объекты XML
    if (!dd_doc.setContent(content, true, errorMessage, &errorLine, &errorColumn)) {

        qDebug("Error in xml structure file: %s\n line %x\n column %x ",
               qPrintable(*errorMessage), errorLine, errorColumn);

        return projectParams;
    }

    // переходим к данным XML
    QDomElement de_root = dd_doc.documentElement();

    QMap<QString, QString> attrs = loadInfo(de_root);
    projectParams.version = attrs["version"];
    projectParams.uuid = attrs["uuid"];

    QDomNode dn_node = de_root.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_node.isNull()) {

        // раздел projectInfo
        if (dn_node.nodeName() == "projectInfo")
            projectParams.projectInfo = loadProjectInfo(dn_node);

        if (dn_node.nodeName() == "events")
            projectParams.events = loadEvents(dn_node);

        if (dn_node.nodeName() == "modules")
            projectParams.modules = loadModules(dn_node);

        if (dn_node.nodeName() == "nodeTypes")
            projectParams.nodeTypes = loadNodeTypes(dn_node);

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
