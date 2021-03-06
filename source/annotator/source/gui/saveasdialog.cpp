#include "saveasdialog.h"
#include "ui_saveasdialog.h"

#include <AnnotatorLib/Storage/StorageFactory.h>

#include <QFileDialog>

SaveAsDialog::SaveAsDialog(std::shared_ptr<AnnotatorLib::Project> project,
                           QWidget *parent)
    : QDialog(parent), ui(new Ui::SaveAsDialog) {
  this->project = project;
  ui->setupUi(this);
  for (std::string saver :
       AnnotatorLib::Storage::StorageFactory::instance()->availableSaver()) {
    ui->fileFormatComboBox->addItem(QString::fromStdString(saver));
  }
}

SaveAsDialog::~SaveAsDialog() { delete ui; }

void SaveAsDialog::on_destinationButton_clicked() {
  QString path;
  if (ui->fileFormatComboBox->currentText() == "xml")
    path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                        tr("Project Files (*.xml)"));
  if (ui->fileFormatComboBox->currentText() == "pascalvocxml")
    path = QFileDialog::getExistingDirectory(this, tr("Export Path"), ".");
  if (ui->fileFormatComboBox->currentText() == "json")
    path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                        tr("Project Files (*.json)"));
  ui->destinationLineEdit->setText(path);
}

void SaveAsDialog::on_buttonBox_accepted() {
  std::string saverType = ui->fileFormatComboBox->currentText().toStdString();
  std::shared_ptr<AnnotatorLib::Storage::AbstractSaver> saver =
      AnnotatorLib::Storage::StorageFactory::instance()->createSaver(saverType);

  std::string path = ui->destinationLineEdit->text().toStdString();
  saver->setPath(path);
  saver->saveProject(project);
  saver->close();
}
