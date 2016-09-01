#include "objectitem.h"
#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Object.h>
#include "ui_objectitem.h"

ObjectItem::ObjectItem(AnnotatorLib::Object *object, QWidget *parent)
    : QWidget(parent), ui(new Ui::ObjectItem), object(object) {
  ui->setupUi(this);
  reload();
}

ObjectItem::~ObjectItem() { delete ui; }

void ObjectItem::reload() {
  this->ui->idLabel->setText(QString::number(this->object->getId()));
  this->ui->nameLineEdit->setText(
      QString::fromStdString(this->object->getName()));
}

AnnotatorLib::Object *ObjectItem::getObject() { return object; }
