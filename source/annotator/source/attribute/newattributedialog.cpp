// Copyright 2016-2017 Annotator Team
#include "newattributedialog.h"
#include "controller/commandcontroller.h"
#include "ui_newattributedialog.h"

#include <memory>

NewAttributeDialog::NewAttributeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewAttributeDialog) {
  ui->setupUi(this);
}

NewAttributeDialog::~NewAttributeDialog() { delete ui; }

std::shared_ptr<AnnotatorLib::Attribute> NewAttributeDialog::getAttribute() {
  return attribute;
}

void NewAttributeDialog::done(int status) {
  if (QDialog::Accepted == status) {
    createAttribute();
  }
  QDialog::done(status);
}

void NewAttributeDialog::createAttribute() {
  std::string name = ui->nameLineEdit->text().toStdString();
  std::string type = ui->typeComboBox->currentText().toStdString();
  std::string value = ui->valueLineEdit->text().toStdString();

  attribute = std::make_shared<AnnotatorLib::Attribute>(type, name);
  std::shared_ptr<AnnotatorLib::AttributeValue> av =
      AnnotatorLib::Attribute::createAttributeValue(type, value);
  attribute->setValue(av);
}
