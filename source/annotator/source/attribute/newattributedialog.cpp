// Copyright 2016 Annotator Team
#include "newattributedialog.h"
#include "ui_newattributedialog.h"

NewAttributeDialog::NewAttributeDialog(
    std::shared_ptr<AnnotatorLib::Session> session,
    shared_ptr<AnnotatorLib::Object> object, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewAttributeDialog), session(session),
      object(object) {
  ui->setupUi(this);
}

NewAttributeDialog::~NewAttributeDialog() { delete ui; }

void NewAttributeDialog::done(int status) {
  if (QDialog::Accepted == status) {
    createAttribute();
  }
  QDialog::done(status);
}

void NewAttributeDialog::createAttribute() {
    std::string name = ui->nameLineEdit->text().toStdString();
    std::string type = ui->typeComboBox->currentText().toStdString();
}
