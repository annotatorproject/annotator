#include "newprojectdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewProjectDialog) {
  ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog() { delete ui; }

std::shared_ptr<AnnotatorLib::Project> NewProjectDialog::getProject() {
  return this->project;
}

QString NewProjectDialog::getStorageType() {
  QString storageType;
  if (ui->jsonRadioButton->isChecked()) storageType = "json";
  if (ui->xmlRadioButton->isChecked()) storageType = "xml";
  if (ui->sqlRadioButton->isChecked()) storageType = "sql";
  return storageType;
}

QString NewProjectDialog::getImageSetType() {
  QString imageSetType;
  if (ui->videoRadioButton->isChecked()) imageSetType = "video";
  if (ui->imagesRadioButton->isChecked()) imageSetType = "images";
  return imageSetType;
}

void NewProjectDialog::on_buttonBox_accepted() {
  QString name = ui->projectNameLineEdit->text();
  QString imageSetType = getImageSetType();
  QString imageSetPath = ui->imageSetPathLineEdit->text();
  QString storageType = getStorageType();
  QString storagePath = ui->storagePathLineEdit->text();
  try {
    project = AnnotatorLib::Project::create(
        name.toStdString(), imageSetType.toStdString(),
        imageSetPath.toStdString(), storageType.toStdString(),
        storagePath.toStdString());
    project->setPath(ui->projectFileLineEdit->text().toStdString());
    project->load();
    this->accept();
  } catch (std::exception &e) {
    QMessageBox::warning(this, "Error", QString::fromStdString(e.what()));
  }
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
    if (!path.endsWith(".pro.xml")) path = path + ".pro.xml";
    ui->projectFileLineEdit->setText(path);
  }
}

void NewProjectDialog::on_storagePathPushButton_clicked() {
  QString path;
  if (getStorageType() == "json") {
    path = QFileDialog::getSaveFileName(this, tr("Select Storage File"), ".",
                                        tr("Json File (*.json)"));
    if (!path.isEmpty() && !path.endsWith(".json")) path = path + ".json";
  } else if (getStorageType() == "xml")
    QMessageBox::warning(this, "Warning", "Not implemented yet");
  // path = QFileDialog::getExistingDirectory(this,tr("Select ImageSet
  // Path"),".");
  if (!path.isEmpty()) ui->storagePathLineEdit->setText(path);
}
