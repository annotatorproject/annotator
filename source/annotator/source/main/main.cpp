#include <QApplication>

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

    return a.exec();
}
