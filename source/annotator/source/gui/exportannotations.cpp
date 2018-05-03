#include "exportannotations.h"
#include "ui_exportannotations.h"

#include <AnnotatorLib/Export/ExportFactory.h>

#include <QFileDialog>

ExportAnnotations::ExportAnnotations(
    std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent)
    : QDialog(parent), ui(new Ui::ExportAnnotations) {
  this->project = project;
  ui->setupUi(this);
  for (std::string exportplugin :
       AnnotatorLib::Export::ExportFactory::instance()->availableExports()) {
    ui->fileFormatComboBox->addItem(QString::fromStdString(exportplugin));
  }
}

ExportAnnotations::~ExportAnnotations() { delete ui; }

void ExportAnnotations::on_destinationButton_clicked() {
  QString path;
  if (ui->fileFormatComboBox->currentText() == "xml")
    path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                        tr("Project Files (*.xml)"));
  else if (ui->fileFormatComboBox->currentText() == "pascalvocxml")
    path = QFileDialog::getExistingDirectory(this, tr("Export Path"), ".");
  else if (ui->fileFormatComboBox->currentText() == "json")
    path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                        tr("Project Files (*.json)"));
  else
    path = QFileDialog::getExistingDirectory(this, tr("Export Path"), ".");
  ui->destinationLineEdit->setText(path);
}

void ExportAnnotations::on_buttonBox_accepted() {
  std::string exportType = ui->fileFormatComboBox->currentText().toStdString();
  std::string path = ui->destinationLineEdit->text().toStdString();

  std::shared_ptr<AnnotatorLib::Export::AbstractExport> exporter =
      AnnotatorLib::Export::ExportFactory::instance()->createExport(
          exportType, project, path);
  exporter->doExport();
}
