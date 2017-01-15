#include "mainwindow.h"
#include <Poco/Exception.h>
#include <QApplication>
#include <QSettings>
#include <sys/stat.h>

#ifdef OPTION_INCLUDE_CRASHCPP
#include <crashcpp/crashcpp.h>
#include <crashcpp/logreport.h>
#include <crashcpp/postreport.h>
#endif

void setStyleSheet(QApplication &app, QString filename) {
  QFile file(filename);
  if (file.open(QFile::ReadOnly)) {
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setOrganizationName("AnnotatorProject");
  a.setApplicationName("Annotator");
  // a.setApplicationDisplayName("Annotation Tool");
  a.setApplicationVersion("Alpha");

#ifdef OPTION_INCLUDE_CRASHCPP
  CrashCpp::init();
  CrashCpp::addReporter(std::make_shared<crashcpp::PostReport>(
      "http://crashcpp.chriamue.de/api/report"));
  CrashCpp::addReporter(std::make_shared<crashcpp::LogReport>());
#endif

  setStyleSheet(a, "annotator.qss");

  MainWindow w;
  w.show();

  // try to open last project
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  QString lastProjPath = settings.value("LastProjectPath", "").toString();

  // check if path was stored previously
  if (!lastProjPath.isEmpty()) {
    // check if file exists on fs
    struct stat buffer;
    if (stat(lastProjPath.toStdString().c_str(), &buffer) == 0)
      w.openProject(AnnotatorLib::Project::load(lastProjPath.toStdString()));
  }

  /*
   * If an unhandled exception occurs, the exception will be sent to crashcpp.chriamue.de
   */
  try {
    return a.exec();
  } catch (cv::Exception &e) {
#ifdef OPTION_INCLUDE_CRASHCPP
    CrashCpp::sendReport("Annotator", "cv::Exception", e.what());
#endif
  } catch (Poco::Exception &e) {
#ifdef OPTION_INCLUDE_CRASHCPP
    CrashCpp::sendReport("Annotator", e.what(), e.message());
#endif
  } catch (std::exception &e) {
#ifdef OPTION_INCLUDE_CRASHCPP
    CrashCpp::sendReport("Annotator", "cv::Exception", e.what());
#endif
  }

  return 1;
}
