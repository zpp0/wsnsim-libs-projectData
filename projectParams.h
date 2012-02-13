/**
 *
 * File: projectParams.h
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#ifndef PROJECTPARAMS_H
#define PROJECTPARAMS_H

#idndef PROJECTPARAMS_VERSION
#warning : Project Params lib version undefined!
#define PROJECTPARAMS_VERSION ""
#endif

// #include <QtCore>
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QList>
#include <QPointer>

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
typedef PluginInfo QMap<QString, QString>;
typedef EventInfo QMap<QString, QString>;
typedef logFileInfo QMap<QString, QString>;
typedef EventArgument QMap<QString, QString>;

class ProjectInfo : public QObject {
    Q_OBJECT
public:
    // name of the author of the project
    QString projectAutor;
    // name of the project
    QString projectName;
    // comment of the project
    QString projectComment;
    // used plugins
    QList<QPointer<PluginInfo> > usedPlugins;
};    

class PluginParams : public QObject {
    Q_OBJECT
public:
    // plugin info
    PluginInfo info;
    // plugin data
    QVariantMap params;
};

class EventParams : public QObject {
    Q_OBJECT
public:
    // event info
    EventInfo info;
    // event arguments
    QList<QPointer<EventArgument> > arguments;
};

class LogFile : public QObject {
    Q_OBJECT
public:
    logFileInfo info;
};

class Events : public QObject {
    Q_OBJECT
public:
    // params
    // TODO: check how it works
    QList<QPointer<EventParams> > pluginsEvents;
    QList<QPointer<EventParams> > userEvents;
};

// agregator
class ProjectParams : public QObject {
    Q_OBJECT
public:
    QString version;
    QPointer<ProjectInfo> projectInfo;
    QList<QPointer<PluginParams> > pluginsParams;
    QPointer<Events> events;
    QList<QPointer<LogFile> > logFiles;
};

typedef errorList QList<QStringList>;

/**
 *
 * Класс для работы с файлом проекта симулятора
 *
 **/

class projectParams : public QObject {
    Q_OBJECT
public:

    // конструктор
    projectParams();

    errorList load(QString& projectFileName);
    QPointer<ProjectParams> getParams();

    void setParams(QPointer<ProjectParams> params);

private:

    // // загрузка информации о проекте из файла проекта в 
    // // возвращает код ошибки
    // void loadProjectInfo();

    // // загрузка параметров плагина из файла проекта
    // // аргумент - имя плагина
    // void loadPluginParams(QString& pluginNamespace, QString& pluginName, QString& pluginVersion);

    // void loadPluginParams(QString& pluginNamespace, QString& pluginName, QString& pluginVersion);

    // void voidErrorHandler(QString& paramsName, QString& error);

    QString m_projectFileName;

    static const QString m_version = PROJECTPARAMS_VERSION;
    
};

#endif // PROJECTPARAMS_H
