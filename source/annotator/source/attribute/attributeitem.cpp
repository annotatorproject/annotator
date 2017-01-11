// Copyright 2016-2017 Annotator Team
#include "attributeitem.h"
#include <memory>
#include "controller/commandcontroller.h"
#include "ui_attributeitem.h"

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/AnnotatorLibDatastructs.h>
#include <AnnotatorLib/Commands/UpdateAttribute.h>

AttributeItem::AttributeItem(shared_ptr<AnnotatorLib::Attribute> attribute,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::AttributeItem), attribute(attribute) {
  ui->setupUi(this);
  ui->updateButton->hide();
  reload();
}

AttributeItem::~AttributeItem() { delete ui; }

shared_ptr<AnnotatorLib::Attribute> AttributeItem::getAttribute() {
  return attribute;
}

void AttributeItem::reload() {
  this->ui->idLabel->setText(QString::number(this->attribute->getId()));
  this->ui->nameLineEdit->setText(
      QString::fromStdString(this->attribute->getName()));
  QString type = QString::fromStdString(
      AnnotatorLib::AttributeTypeToString(this->attribute->getType()));
  this->ui->typeComboBox->setCurrentText(type);
  QString defaultValue =
      QString::fromStdString(this->attribute->getValue()->toString());
  this->ui->valueLineEdit->setText(defaultValue);
}

void AttributeItem::setAnnotation(
    shared_ptr<AnnotatorLib::Annotation> annotation) {
  this->annotation = annotation;
}

void AttributeItem::on_typeComboBox_currentIndexChanged(const QString &arg1) {
  if (annotation) ui->updateButton->show();
}

void AttributeItem::on_valueLineEdit_editingFinished() {
  if (annotation) ui->updateButton->show();
}

void AttributeItem::on_updateButton_clicked() {
  if (annotation) {
    std::string value = ui->valueLineEdit->text().toStdString();
    std::shared_ptr<AnnotatorLib::Annotation> a = annotation;
    shared_ptr<AnnotatorLib::Commands::UpdateAttribute> nAttr =
        std::make_shared<AnnotatorLib::Commands::UpdateAttribute>(
            annotation, attribute->getName(), value);
    CommandController::instance()->execute(nAttr);
    ui->updateButton->hide();
  }
}
