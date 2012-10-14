/**
 *
 * File: projectData_save.cpp
 * Description: methods to saveing project data to XML
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include "projectData.h"

void ProjectData::createXml(QDomDocument* result, QDomElement* parent, QString XNodeName, QString XNodeValue, QMap<QString, QString> attrs)
{
    QDomElement de_new = result->createElement(XNodeName);
    foreach (QString attrName, attrs.keys())
        de_new.setAttribute(attrName, attrs[attrName]);

    QDomText dt_new = result->createTextNode(XNodeValue);
    de_new.appendChild(dt_new);
    parent->appendChild(de_new);
}

void ProjectData::createXml(QDomDocument* result, QDomElement* parent, QString XNodeName, QString XNodeValue)
{
    QDomElement de_new = result->createElement(XNodeName);

    QDomText dt_new = result->createTextNode(XNodeValue);
    de_new.appendChild(dt_new);
    parent->appendChild(de_new);
}

void ProjectData::saveProjectInfo(QDomDocument* result, QDomElement* parent, ProjectInfo info)
{
    QDomElement de_projectInfo = result->createElement("projectInfo");

    createXml(result, &de_projectInfo, "author", info.projectAutor);
    createXml(result, &de_projectInfo, "title", info.projectTitle);
    createXml(result, &de_projectInfo, "comment", info.projectComment);

    createXml(result, &de_projectInfo, "keywords", info.keywords.join(" "));
    createXml(result, &de_projectInfo, "revision", QString::number(info.revision));
    createXml(result, &de_projectInfo, "modified", QString::number(info.modified));

    parent->appendChild(de_projectInfo);
}

void ProjectData::saveSimulatorParams(QDomDocument* result, QDomElement* parent, SimulatorParams simParams)
{
    QDomElement de_tree = result->createElement("simulatorParams");

    createXml(result, &de_tree, "maxTime", QString::number(simParams.maxTime));
    createXml(result, &de_tree, "timeUnits", QString::number(simParams.timeUnits));
    createXml(result, &de_tree, "logFile", simParams.logFile);

    parent->appendChild(de_tree);
}

void ProjectData::saveEventParams(QDomDocument* result, QDomElement* parent, EventParams eventParams)
{
    QDomElement de_tree = result->createElement("event");
    foreach (QString param, eventParams.eventInfo.keys())
        de_tree.setAttribute(param, eventParams.eventInfo[param]);

    foreach (EventArgument argument, eventParams.arguments)
        createXml(result, &de_tree, "argument", "", argument);

    parent->appendChild(de_tree);
}

void ProjectData::saveEvents(QDomDocument* result, QDomElement* parent, Events events)
{
    QDomElement de_tree_ = result->createElement("events");
    QDomElement de_tree = result->createElement("systemEvents");

    foreach (EventParams eventParams, events.systemEvents)
        saveEventParams(result, &de_tree, eventParams);

    de_tree_.appendChild(de_tree);
    parent->appendChild(de_tree_);
}

void ProjectData::saveModules(QDomDocument* result, QDomElement* parent, QList<ModuleData> modules)
{
    QDomElement de_tree = result->createElement("modules");

    for (int moduleID = 0; moduleID < modules.size(); moduleID++) {
        ModuleData module = modules[moduleID];

        // creating module subtree
        QDomElement de_module = result->createElement("module");

        // setting ID attribute
        de_module.setAttribute("ID", moduleID);
        // setting others attributes
        foreach (QString attr, module.moduleInfo.keys())
            de_module.setAttribute(attr, module.moduleInfo[attr]);

        // saving fileName node
        createXml(result, &de_module, "fileName", module.fileName);
        // saving module params
        saveModulesParams(result, &de_module, module.params);

        // creating dependences subtree
        QDomElement de_dependencies = result->createElement("dependencies");

        foreach (QString moduleName, module.dependencies.keys()) {
            QMap<QString, QString> paramsAttrs;
            paramsAttrs["name"] = moduleName;
            createXml(result, &de_dependencies, "module", QString::number(module.dependencies[moduleName]));
        }

        // append dependencies subtree to module subtree
        de_module.appendChild(de_dependencies);
        // append module subtree to global tree
        de_tree.appendChild(de_module);
    }

    parent->appendChild(de_tree);
}

void ProjectData::saveModulesParams(QDomDocument* result, QDomElement* parent, QList<ModuleParam> moduleParams)
{
    QDomElement de_tree = result->createElement("params");

    foreach (ModuleParam param, moduleParams) {

        QMap<QString, QString> paramsAttrs;
        paramsAttrs["name"] = param.name;
        paramsAttrs["type"] = param.type;

        if (param.type == "table") {

            // TODO: implement this
        }
        if (param.type == "Probability distribution") {
            // TODO: implement this
        }
        else
            createXml(result, &de_tree, "param", param.value.toString(), paramsAttrs);

    }

    parent->appendChild(de_tree);
}

void ProjectData::saveLogFiles(QDomDocument* result, QDomElement* parent, QList<LogFileInfo> logFilesInfo)
{
    QDomElement de_tree = result->createElement("logFiles");

    foreach (LogFileInfo logFileInfo, logFilesInfo)
        createXml(result, &de_tree, "logFile", "", logFileInfo);

    parent->appendChild(de_tree);
}

void ProjectData::save(QString& projectFileName, QString* errorMessage, ProjectParams params)
{
    QFile file(projectFileName);

    // создаем объект с данными XML
    // в него сначала будут записаны все данные, потом он записывается в файл
    QDomDocument result;
    // создаем узел main, дочерними которого будут все остальные узлы
    QDomElement de_resultElement = result.createElement("project");
    de_resultElement.setAttribute("version", params.version);

    // сохраняем параметры проекта
    saveProjectInfo(&result, &de_resultElement, params.projectInfo);
    // сохраняем параметры симулятора
    saveSimulatorParams(&result, &de_resultElement, params.simulatorParams);

    saveModules(&result, &de_resultElement, params.modules);

    saveEvents(&result, &de_resultElement, params.events);

    saveLogFiles(&result, &de_resultElement, params.logFiles);

    // открываем файл на запись
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // пишем данные из объекта xml в файл

        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("UTF-8"));

        QDomNode xmlNode = result.createProcessingInstruction("xml",
                                                              "version=\"1.0\" encoding=\"UTF-8\"");
        result.appendChild(xmlNode);
        result.appendChild(de_resultElement);
        out << result.toString(4);
    }

    file.close();
}
