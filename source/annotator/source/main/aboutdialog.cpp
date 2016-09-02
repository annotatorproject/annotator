#include "aboutdialog.h"
#include "annotator.h"
#include "ui_aboutdialog.h"

#include <AnnotatorLib/AnnotatorLib.h>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  ui->annotatorVersionLabel->setText(
      "Version: " + QString::fromStdString(Annotator::VERSION));
  ui->annotatorLibVersionLabel->setText(
      "Version: " + QString::fromStdString(AnnotatorLib::VERSION));
}

AboutDialog::~AboutDialog() { delete ui; }

void AboutDialog::on_okButton_clicked() { accept(); }
