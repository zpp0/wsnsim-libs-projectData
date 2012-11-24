#include "projectData.h"

bool ProjectData::isInjectedFormatsSettings(QDomElement de_root)
{
    QDomNode dn_node = de_root.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_node.isNull()) {
        if (dn_node.nodeName() == "formatsSettings")
            return true;

        // переходим к следующему узлу XML
        dn_node = dn_node.nextSibling();
    }

    return false;
}

bool ProjectData::isInjectedColumnsSettings(QDomElement de_root)
{
    QDomNode dn_node = de_root.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_node.isNull()) {
        if (dn_node.nodeName() == "columnsSettings")
            return true;

        // переходим к следующему узлу XML
        dn_node = dn_node.nextSibling();
    }

    return false;
}

VisualizationInfo ProjectData::loadVisualizationInfo(QDomElement de_root)
{
    VisualizationInfo visualizationInfo;

    QDomNode dn_node = de_root.firstChild();

    // перебираем узлы, пока не закончатся
    while (!dn_node.isNull()) {
        if (dn_node.nodeName() == "formatsSettings")
        {
            QDomElement element = dn_node.toElement();
            QString value = element.attribute("value");

            visualizationInfo.formatsSettings = value;
        }

        if (dn_node.nodeName() == "columnsSettings")
        {
            QDomElement element = dn_node.toElement();
            QString value = element.attribute("value");

            visualizationInfo.columnsSettings = value;
        }

        dn_node = dn_node.nextSibling();
    }

    return visualizationInfo;
}

void ProjectData::saveVisualizationInfo(QDomDocument *result, QDomElement *parent, VisualizationInfo visualizationInfo)
{
    QDomElement de_formats = result->createElement("formatsSettings");
    de_formats.setAttribute("value", visualizationInfo.formatsSettings);
    parent->appendChild(de_formats);

    QDomElement de_columns = result->createElement("columnsSettings");
    de_columns.setAttribute("value", visualizationInfo.columnsSettings);
    parent->appendChild(de_columns);
}
