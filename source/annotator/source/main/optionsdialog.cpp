// Copyright 2017 Annotator Team

#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <AnnotatorLib/Storage/StorageFactory.h>
#include <QFileDialog>
#include <QSettings>

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::OptionsDialog) {
  ui->setupUi(this);
  loadOptions();
}

OptionsDialog::~OptionsDialog() { delete ui; }

void OptionsDialog::loadOptions() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  QString storagesPath = settings.value("StoragesPath", "storages").toString();
  ui->storagesLineEdit->setText(storagesPath);
}

void OptionsDialog::on_buttonBox_accepted() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("StoragesPath", ui->storagesLineEdit->text());
  AnnotatorLib::Storage::StorageFactory::instance()->loadPlugins(
      ui->storagesLineEdit->text().toStdString());
}

void OptionsDialog::on_storagesButton_clicked() {
  QString storagesPath = QFileDialog::getExistingDirectory(
      this, tr("storage plugins path"), ui->storagesLineEdit->text());
  ui->storagesLineEdit->setText(storagesPath);
}
