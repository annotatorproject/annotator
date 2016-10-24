#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <AnnotatorLib/Project.h>
#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include "annotation/annotationswidget.h"
#include "attribute/attributeswidget.h"
#include "object/objectswidget.h"
#include "object/selectedobject.h"
#include "player.h"
#include "plugins/pluginswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void connectSignalSlots();

  void setWindowTitle(std::shared_ptr<AnnotatorLib::Project> project);

  void setRateValue(QString value);

  void openProject(std::shared_ptr<AnnotatorLib::Project> project);

 protected:
  /**
   * @brief loadRecentProjects
   * Loads recent projects files from settings and creates menu items for them.
   */
  void loadRecentProjects();

  /**
   * @brief addRecentProject
   * @param projectfile The project file to save in settings.
   *
   */
  void addRecentProject(QString projectfile);

 signals:
  void signal_refreshSession();

 public slots:
  void closeEvent(QCloseEvent *event);

 private slots:
  void on_actionOpen_Project_triggered();

  void on_actionClose_Project_triggered();

  void on_actionNew_Project_triggered();

  void on_actionQuit_triggered();

  void on_actionSave_Project_triggered();

  void on_actionUndo_triggered();

  void on_actionRedo_triggered();

  void on_actionCompress_Session_triggered();

  void on_actionClear_Session_triggered();

  void on_actionAbout_triggered();

  void on_actionClasses_triggered();

  void on_actionProject_Statistics_triggered();

  void on_actionLock_project_toggled(bool arg1);

  void on_actionRun_Plugins_Dialog_triggered();

 protected:
  Player playerWidget;
  AnnotationsWidget annotationsWidget;
  ObjectsWidget objectsWidget;
  SelectedObject selectedObject;
  AttributesWidget attributesWidget;
  PluginsWidget pluginsWidget;

  std::shared_ptr<AnnotatorLib::Project> project = nullptr;
  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
  QLabel *rateLabel;

  void enableDrawing(bool enable);
  void lock(bool b);
  void closeProject();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
