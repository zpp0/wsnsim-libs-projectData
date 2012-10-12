/**
 *
 * File: projectData.cpp
 * Description: saveing and loading project data in XML
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
 *
 **/

#include <QTextCodec>
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
    // устанавливаем кодировки для строк
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
}

extern "C" MY_EXPORT ProjectParams load(QString& projectFileName, QString* errorMessage)
{
    ProjectData projectData;
    return projectData.load(projectFileName, errorMessage);
}

extern "C" MY_EXPORT void save(QString& projectFileName, QString* errorMessage, ProjectParams params)
{
    ProjectData projectData;
    projectData.save(projectFileName, errorMessage, params);
}
