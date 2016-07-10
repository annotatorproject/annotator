#include "attributeitem.h"
#include "ui_attributeitem.h"

#include <AnnotatorLib/AnnotatorLibDatastructs.h>

AttributeItem::AttributeItem(AnnotatorLib::Attribute *attribute,
                             QWidget *parent)
    : QWidget(parent), attribute(attribute), ui(new Ui::AttributeItem) {
  ui->setupUi(this);
  reload();
}

AttributeItem::~AttributeItem() { delete ui; }

void AttributeItem::reload() {
  this->ui->idLabel->setText(QString::number(this->attribute->getId()));
  this->ui->nameLineEdit->setText(
      QString::fromStdString(this->attribute->getName()));
  QString type = QString::fromStdString(
      AnnotatorLib::AttributeTypeToString(this->attribute->getType()));
  this->ui->typeComboBox->setCurrentText(type);
  QString defaultValue =
      QString::fromStdString(this->attribute->getDefaultValue()->toString());
  this->ui->valueLineEdit->setText(defaultValue);
}
