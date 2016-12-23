#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

NewProjectDialog::NewProjectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewProjectDialog) {
  ui->setupUi(this);
  connect(ui->imageSetPathLineEdit, SIGNAL(textChanged(QString)), this,
          SLOT(checkLineEdits()));
  connect(ui->projectFileLineEdit, SIGNAL(textChanged(QString)), this,
          SLOT(checkLineEdits()));
  connect(ui->storagePathLineEdit, SIGNAL(textChanged(QString)), this,
          SLOT(checkLineEdits()));
  connect(ui->projectNameLineEdit, SIGNAL(textChanged(QString)), this,
          SLOT(checkLineEdits()));
}

NewProjectDialog::~NewProjectDialog() { delete ui; }

std::shared_ptr<AnnotatorLib::Project> NewProjectDialog::getProject() {
  return this->project;
}

QString NewProjectDialog::getStorageType() {
  QString storageType;
  if (ui->jsonRadioButton->isChecked())
    storageType = "json";
  if (ui->xmlRadioButton->isChecked())
    storageType = "xml";
  if (ui->sqlRadioButton->isChecked())
    storageType = "sql";
  return storageType;
}

QString NewProjectDialog::getImageSetType() {
  QString imageSetType;
  if (ui->videoRadioButton->isChecked())
    imageSetType = "video";
  if (ui->imagesRadioButton->isChecked())
    imageSetType = "images";
  return imageSetType;
}

void NewProjectDialog::on_imageSetPathPushButton_clicked() {
  QString path;
  if (getImageSetType() == "video")
    // TODO more video file formats
    path = QFileDialog::getOpenFileName(
        this, tr("Select Video File"), ".",
        tr("Video Files (*.mpg *.mpeg *.mp4 *.avi *.ogg)"));
  else if (getImageSetType() == "images")
    path = QFileDialog::getExistingDirectory(this, tr("Select ImageSet Path"),
                                             ".");
  if (!path.isEmpty()) {
    ui->imageSetPathLineEdit->setText(path);
  }
}

void NewProjectDialog::on_projectFilePushButton_clicked() {
  QString path;
  path = QFileDialog::getSaveFileName(this, tr("Select Project File"), ".",
                                      tr("Annotator Project File (*.pro.xml)"));
  if (!path.isEmpty()) {
    if (!path.endsWith(".pro.xml"))
      path = path + ".pro.xml";
    ui->projectFileLineEdit->setText(path);
  }
}

void NewProjectDialog::on_storagePathPushButton_clicked() {
  QString path;
  if (getStorageType() == "json") {
    path = QFileDialog::getSaveFileName(this, tr("Select Storage File"), ".",
                                        tr("Json File (*.json)"));
    if (!path.isEmpty() && !path.endsWith(".json"))
      path = path + ".json";
  } else if (getStorageType() == "xml")
    QMessageBox::warning(this, "Warning", "Not implemented yet");

  if (!path.isEmpty())
    ui->storagePathLineEdit->setText(path);
}

void NewProjectDialog::checkLineEdits() {
  bool ok = !ui->imageSetPathLineEdit->text().isEmpty() &&
            !ui->projectFileLineEdit->text().isEmpty() &&
            !ui->storagePathLineEdit->text().isEmpty() &&
            !ui->projectNameLineEdit->text().isEmpty();
  ui->pushButtonOk->setEnabled(ok);
}

void NewProjectDialog::on_pushButtonOk_clicked() {
  std::string name = ui->projectNameLineEdit->text().toStdString();
  std::string imageSetType = getImageSetType().toStdString();
  std::string imageSetPath = ui->imageSetPathLineEdit->text().toStdString();
  std::string storageType = getStorageType().toStdString();
  std::string storagePath = ui->storagePathLineEdit->text().toStdString();
  std::string projectFilePath = ui->projectFileLineEdit->text().toStdString();
  try {
    project = AnnotatorLib::Project::create(name, imageSetType, imageSetPath,
                                            storageType, storagePath);
    project->setPath(projectFilePath);
    project->create();
    project->load();
    this->accept();
  } catch (std::exception &e) {
    QMessageBox::warning(this, "Error", QString::fromStdString(e.what()));
  }
}

void NewProjectDialog::on_sqlRadioButton_toggled(bool checked) {
  if (checked)
    ui->storagePathLineEdit->setPlaceholderText(
        "host=localhost;user=annotator;password=annotator;db=annotatordb;");
}

void NewProjectDialog::on_jsonRadioButton_toggled(bool checked) {
  if (checked)
    ui->storagePathLineEdit->setPlaceholderText("storage.json");
}
