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
    createXml(result, &de_projectInfo, "modified", info.modified);

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

        foreach (ModuleDependence moduleDep, module.dependencies) {

            QDomElement de_dependence = result->createElement("dependence");

            de_dependence.setAttribute("name", moduleDep.name);
            de_dependence.setAttribute("type", moduleDep.type);
            de_dependence.setAttribute("hasFunctions", moduleDep.hasFunctions);
            de_dependence.setAttribute("ID", QString::number(moduleDep.moduleID));

            foreach(QString event, moduleDep.events.keys())
                foreach(EventArgument param, moduleDep.events[event])
                    createXml(result, &de_dependence, "param", event, param);

            // append dependencies subtree to module subtree
            de_dependencies.appendChild(de_dependence);

        }

        // append module subtree to global tree

        de_module.appendChild(de_dependencies);
        de_tree.appendChild(de_module);
    }

    parent->appendChild(de_tree);
}

void ProjectData::saveNodes(QDomDocument* result, QDomElement* parent, QList<NodesData> nodes)
{
    QDomElement de_nodes = result->createElement("nodesNumber");
    foreach(NodesData nodesData, nodes) {
        QMap<QString, QString> paramsAttrs;
        paramsAttrs["moduleID"] = QString::number(nodesData.moduleID);
        paramsAttrs["nodeType"] = nodesData.nodeType;
        createXml(result, &de_nodes, "nodes", QString::number(nodesData.nodesNumber), paramsAttrs);
    }

    parent->appendChild(de_nodes);
}

void ProjectData::saveNodeTypes(QDomDocument* result, QDomElement* parent, QList<NodeTypeData> nodeTypes)
{
    QDomElement de_tree = result->createElement("nodeTypes");

    foreach(NodeTypeData nodeType, nodeTypes) {
        QDomElement de_nodeType = result->createElement("nodeType");

        // bind the name attribute
        de_nodeType.setAttribute("name", nodeType.name);

        QDomElement de_hardware = result->createElement("hardware");
        foreach(quint16 moduleID, nodeType.hardwareModules)
            createXml(result, &de_hardware, "module", QString::number(moduleID));

        QDomElement de_software = result->createElement("software");
        foreach(quint16 moduleID, nodeType.softwareModules)
            createXml(result, &de_software, "module", QString::number(moduleID));

        de_nodeType.appendChild(de_hardware);
        de_nodeType.appendChild(de_software);

        de_tree.appendChild(de_nodeType);
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
            QDomElement de_param = result->createElement("param");

            de_param.setAttribute("name", param.name);
            de_param.setAttribute("type", param.type);

            QMap<QString, QVariant> value = param.value.toMap();
            foreach(QString row, value.keys()) {

                QDomElement de_row = result->createElement("row");
                de_row.setAttribute("num", row);

                foreach(QString column, value[row].toMap().keys()) {
                    QMap<QString, QString> attrs;
                    attrs["name"] = column;
                    QString svalue = QString::number(value[row].toMap()[column].toDouble());
                    createXml(result, &de_row, "column", svalue, attrs);
                }

                de_param.appendChild(de_row);
            }
            de_tree.appendChild(de_param);
        }

        else if (param.type == "Probability distribution") {
            // TODO: implement this
        }

        else if (param.type == "nodes") {
            QDomElement de_param = result->createElement("param");

            de_param.setAttribute("name", param.name);
            de_param.setAttribute("type", param.type);

            QVariantMap value = param.value.toMap();

            foreach(QString nodeType, value.keys()) {
                QMap<QString, QString> attrs;
                attrs["name"] = nodeType;
                createXml(result, &de_param, "nodeType", QString::number(value[nodeType].toInt()), attrs);
            }
            de_tree.appendChild(de_param);

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

void ProjectData::saveIndexFileInfo(QDomDocument *result, QDomElement *parent, IndexFileInfo indexFileInfo)
{
    QDomElement de_tree = result->createElement("indexFileInfo");

    createXml(result, &de_tree, "indexFile", "", indexFileInfo);

    parent->appendChild(de_tree);
}

void ProjectData::saveVisualizationInfo(QDomDocument *result, QDomElement *parent, VisualizationInfo visualizationInfo)
{
    QDomElement de_visualizationInfo = result->createElement("visualizationInfo");

    QDomElement de_formats = result->createElement("formatsSettings");
    de_formats.setAttribute("value", visualizationInfo.formatsSettings);
    de_visualizationInfo.appendChild(de_formats);

    QDomElement de_columns = result->createElement("columnsSettings");
    de_columns.setAttribute("value", visualizationInfo.columnsSettings);
    de_visualizationInfo.appendChild(de_columns);

    parent->appendChild(de_visualizationInfo);
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
    de_resultElement.setAttribute("uuid", params.uuid);

    // сохраняем параметры проекта
    saveProjectInfo(&result, &de_resultElement, params.projectInfo);
    // сохраняем параметры симулятора
    saveSimulatorParams(&result, &de_resultElement, params.simulatorParams);

    saveModules(&result, &de_resultElement, params.modules);

    saveNodes(&result, &de_resultElement, params.nodes);

    saveNodeTypes(&result, &de_resultElement, params.nodeTypes);

    saveEvents(&result, &de_resultElement, params.events);

    saveLogFiles(&result, &de_resultElement, params.logFiles);

    saveIndexFileInfo(&result, &de_resultElement, params.indexFileInfo);

    saveVisualizationInfo(&result, &de_resultElement, params.visualizationInfo);

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
