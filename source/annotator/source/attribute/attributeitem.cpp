#include "attributeitem.h"
#include "ui_attributeitem.h"

#include <AnnotatorLib/AnnotatorLibDatastructs.h>

AttributeItem::AttributeItem(shared_ptr<AnnotatorLib::Attribute> attribute,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::AttributeItem), attribute(attribute) {
  ui->setupUi(this);
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
