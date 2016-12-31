#include "objectitem.h"
#include <AnnotatorLib/Class.h>
#include <assert.h>
#include "objectwindow.h"
#include "ui_objectitem.h"

ObjectItem::ObjectItem(std::shared_ptr<AnnotatorLib::Session> session,
                       shared_ptr<AnnotatorLib::Object> object, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ObjectItem),
      session(session),
      object(object) {
  assert(object != nullptr);
  ui->setupUi(this);
  reload();
}

ObjectItem::~ObjectItem() { delete ui; }

void ObjectItem::reload() {
  this->ui->idLabel->setText(QString::number(this->object->getId()));
  this->ui->nameLineEdit->setText(
      QString::fromStdString(this->object->getName()));
}

shared_ptr<AnnotatorLib::Object> ObjectItem::getObject() { return object; }

void ObjectItem::on_windowButton_clicked() {
  ObjectWindow wnd(this->session, this->object);
  wnd.exec();
}
