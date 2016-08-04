#include <QApplication>
#include <sys/stat.h>
#include "mainwindow.h"

void setStyleSheet(QApplication &app, QString filename){
    QFile file(filename);
    if(file.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Annotator");
    //a.setApplicationDisplayName("Annotation Tool");
    a.setApplicationVersion("Alpha");

    setStyleSheet(a, "annotator.qss");

    MainWindow w;
    w.show();

    //try to open last project
    //TODO: ORG_NAME
    QSettings settings("ORG_NAME", "annotator-demo");
    QString lastProjPath = settings.value("LastProjectPath","").value<QString>();

    //check if path was stored previously
    if (!lastProjPath.isEmpty())
    {
        //check if file exists on fs
        struct stat buffer;
        if(stat (lastProjPath.toStdString().c_str(), &buffer) == 0) w.openProject(AnnotatorLib::Project::load(lastProjPath.toStdString()));
    }


    return a.exec();
}
