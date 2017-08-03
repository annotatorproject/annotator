// Copyright 2017 Annotator Team

#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <AnnotatorLib/ImageSet/ImageSetFactory.h>
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
  QString imagesetsPath =
      settings.value("ImageSetsPath", "imagesets").toString();
  ui->storagesLineEdit->setText(storagesPath);
}

void OptionsDialog::on_buttonBox_accepted() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("StoragesPath", ui->storagesLineEdit->text());
  AnnotatorLib::Storage::StorageFactory::instance()->loadPlugins(
      ui->storagesLineEdit->text().toStdString());
  settings.setValue("ImageSetsPath", ui->imagesetsLineEdit->text());
  AnnotatorLib::ImageSet::ImageSetFactory::instance()->loadPlugins(
      ui->imagesetsLineEdit->text().toStdString());
}

void OptionsDialog::on_storagesButton_clicked() {
  QString storagesPath = QFileDialog::getExistingDirectory(
      this, tr("storage plugins path"), ui->storagesLineEdit->text());
  ui->storagesLineEdit->setText(storagesPath);
}

void OptionsDialog::on_imagesetsButton_clicked() {
  QString imagesetsPath = QFileDialog::getExistingDirectory(
      this, tr("imageset plugins path"), ui->imagesetsLineEdit->text());
  ui->imagesetsLineEdit->setText(imagesetsPath);
}
