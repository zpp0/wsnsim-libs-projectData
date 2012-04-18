#include <QtCore>
#include <QLibrary>

#include "../projectData.h"

int main(int argc, char *argv[])
{
    QLibrary projectDataLib("../libprojectData");
    projectDataLib.load();

    if(!projectDataLib.isLoaded()) {
        qDebug() << "Error load library";
        return 1;
    }        

    // FIXME: make it easy
    typedef ProjectParams(*projectDataLoad) (QString& projectFileName, QString* errorMessage);
    projectDataLoad pd = (projectDataLoad) projectDataLib.resolve("load");

    QString errorMessage;
    QString projectFile;

    if (argc > 1)
        projectFile = argv[1];
    else
        projectFile = "events.xml";

    // FIXME: если убрать {}, то будет сегфолт
    {
        ProjectParams projectParams = pd(projectFile, &errorMessage);
    }

    return 0;
}
