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
    a.setOrganizationName("AnnotatorProject");
    a.setApplicationName("Annotator");
    //a.setApplicationDisplayName("Annotation Tool");
    a.setApplicationVersion("Alpha");

    setStyleSheet(a, "annotator.qss");

    MainWindow w;
    w.show();

    //try to open last project
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QString lastProjPath = settings.value("LastProjectPath","").toString();

    //check if path was stored previously
    if (!lastProjPath.isEmpty())
    {
        //check if file exists on fs
        struct stat buffer;
        if(stat (lastProjPath.toStdString().c_str(), &buffer) == 0) w.openProject(AnnotatorLib::Project::load(lastProjPath.toStdString()));
    }

    return a.exec();
}