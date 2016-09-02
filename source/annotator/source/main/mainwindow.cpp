#include "mainwindow.h"
#include <string>
#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Commands/CompressSession.h>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "aboutdialog.h"
#include "controller/commandcontroller.h"
#include "gui/classesdialog.h"
#include "newprojectdialog.h"
#include "plugins/pluginloader.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->PlayerLayout->addWidget(&playerWidget);
  ui->annotationsLayout->addWidget(&annotationsWidget);
  ui->objectsLayout->addWidget(&objectsWidget);
  // ui->attributesLayout->addWidget(&attributesWidget); // TODO
  ui->pluginsLayout->addWidget(&pluginsWidget);

  rateLabel = new QLabel;     // Frame rate
  rateLabel->setText("fps");  // Add to Status bar

  // make some initialization
  ui->statusBar->addPermanentWidget(rateLabel);
  playerWidget.setRateLabel(rateLabel);
  loadRecentProjects();
  connectSignalSlots();
}

MainWindow::~MainWindow() {
  if (this->project != nullptr) {
    delete this->project;
  }
  delete ui;
}

void MainWindow::connectSignalSlots() {
  connect(&annotationsWidget, SIGNAL(objectSelected(AnnotatorLib::Object *)),
          &objectsWidget, SLOT(selectObject(AnnotatorLib::Object *)));

  connect(&annotationsWidget, SIGNAL(frameSelected(long)), &playerWidget,
          SLOT(jumpTo(long)));

  connect(&playerWidget, SIGNAL(objectSelected(AnnotatorLib::Object *)),
          &objectsWidget, SLOT(selectObject(AnnotatorLib::Object *)));

  connect(&playerWidget, SIGNAL(requestReload()), this, SLOT(reloadWidgets()));
}

void MainWindow::setRateValue(QString value) { rateLabel->setText(value); }

void MainWindow::openProject(AnnotatorLib::Project *project) {
  if (project != nullptr) {
    this->project = project;

    this->session = project->getSession();
    CommandController::instance()->setSession(session);
    playerWidget.setProject(project);

    annotationsWidget.setSession(this->session);
    objectsWidget.setSession(this->session);
    attributesWidget.setSession(this->session);

    this->setRateValue(playerWidget.getRateValue());
    setWindowTitle(project);

    reloadWidgets();

    for (Annotator::Plugin *p :
         Annotator::PluginLoader::getInstance().getPlugins()) {
      p->setSession(project->getSession());
    }

    // save path to last opened project in settings
    addRecentProject(QString::fromStdString(project->getPath()));
  }
}

void MainWindow::loadRecentProjects() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  foreach (QVariant v, settings.value("RecentProjects").toList()) {
    QFileInfo fileInfo(v.toString());

    QAction *recentProjectAction =
        new QAction(fileInfo.baseName(), ui->menuRecentProjects);
    recentProjectAction->setToolTip(v.toString());
    // load project when action is triggered
    connect(recentProjectAction, &QAction::triggered, this,
            [recentProjectAction, this]() {
              QString recentProject = recentProjectAction->toolTip();
              this->openProject(
                  AnnotatorLib::Project::load(recentProject.toStdString()));
            });
    ui->menuRecentProjects->addAction(recentProjectAction);
  }
}

void MainWindow::addRecentProject(QString projectfile) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());

  QVariantList recentProjects = settings.value("RecentProjects").toList();
  if (recentProjects.contains(projectfile))
    recentProjects.removeOne(projectfile);
  recentProjects.prepend(projectfile);
  settings.setValue("RecentProjects", recentProjects);
  settings.setValue("LastProjectPath", projectfile);
}

void MainWindow::reloadWidgets() {
  annotationsWidget.reload();
  objectsWidget.reload();
  attributesWidget.reload();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (this->project == nullptr) {
    event->accept();
    return;
  }

  QMessageBox::StandardButton resBtn = QMessageBox::question(
      this, "", tr("Save project before quit?\n"),
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
      QMessageBox::Yes);
  if (resBtn == QMessageBox::Cancel) {
    event->ignore();
  } else {
    if (resBtn == QMessageBox::Yes) project->saveSession();
    event->accept();
  }
}

void MainWindow::setWindowTitle(AnnotatorLib::Project *project) {
  if (project == nullptr) {
    QMainWindow::setWindowTitle(QApplication::applicationName());
  } else {
    QMainWindow::setWindowTitle(QApplication::applicationName() + " - " +
                                QString::fromStdString(project->getName()));
  }
}

void MainWindow::on_actionOpen_Project_triggered() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), ".",
                                                  tr("Project Files (*.xml)"));
  if (QFile::exists(fileName)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    Annotator::PluginLoader::getInstance();
    openProject(AnnotatorLib::Project::load(fileName.toStdString()));
    QApplication::restoreOverrideCursor();
  }
}

void MainWindow::on_actionClose_Project_triggered() {
  QMessageBox::StandardButton resBtn = QMessageBox::question(
      this, "", tr("Save project before close?\n"),
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
      QMessageBox::Yes);
  if (resBtn == QMessageBox::Cancel) {
  } else {
    if (resBtn == QMessageBox::Yes) project->saveSession();
    if (this->project != nullptr) {
      delete this->project;
    }
  }
}

void MainWindow::on_actionNew_Project_triggered() {
  NewProjectDialog dialog;
  dialog.exec();
  if (dialog.getProject() != nullptr) {
    this->project = dialog.getProject();
    AnnotatorLib::Project::save(this->project, this->project->getPath());
    openProject(this->project);
  }
}

void MainWindow::on_actionQuit_triggered() {
  if (!playerWidget.videoplayer->isStop()) {
    playerWidget.videoplayer->pauseIt();
  }
  this->close();
}

void MainWindow::on_actionSave_Project_triggered() {
  if (this->project != nullptr) {
    project->save();
  }
}

void MainWindow::on_actionUndo_triggered() {
  if (this->project != nullptr) {
    CommandController::instance()->undo();
  }
}

void MainWindow::on_actionRedo_triggered() {
  if (this->project != nullptr) {
    CommandController::instance()->redo();
  }
}

void MainWindow::on_actionAuto_Annotate_toggled(bool arg1) {
  this->playerWidget.setAutoAnnotation(arg1);
}

void MainWindow::on_actionCompress_Session_triggered() {

  //TODO: freeze window and show progress bar
  int nmb_annotations_before = session->getAnnotations().size();
  AnnotatorLib::Commands::CompressSession *cmd =
      new AnnotatorLib::Commands::CompressSession(this->session);
  CommandController::instance()->execute(cmd);
  int nmb_annotations_after= session->getAnnotations().size();

  QMessageBox::question( this, "",
                         QString::fromStdString(std::string("Compression algorithm has removed ") +
                          std::to_string(nmb_annotations_before - nmb_annotations_after)
                          + std::string(" annotations.\n")),
                          QMessageBox::Ok);
}

void MainWindow::on_actionAbout_triggered() {
  AboutDialog aboutDialog;
  aboutDialog.exec();
}

void MainWindow::on_actionClasses_triggered() {
  if (session != nullptr) {
    ClassesDialog dialog(session, this);
    dialog.exec();
  }
}
