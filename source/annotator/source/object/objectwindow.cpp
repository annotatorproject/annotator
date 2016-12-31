// Copyright 2016 Annotator Team
#include "objectwindow.h"
#include "../attribute/attributeitem.h"
#include "../attribute/newattributedialog.h"
#include "../gui/classesdialog.h"
#include "ui_objectwindow.h"

#include <assert.h>

#include <AnnotatorLib/Attribute.h>
#include <AnnotatorLib/Class.h>

#include <QMessageBox>

ObjectWindow::ObjectWindow(std::shared_ptr<AnnotatorLib::Session> session,
                           shared_ptr<AnnotatorLib::Object> object,
                           QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ObjectWindow),
      session(session),
      object(object) {
  assert(object != nullptr);
  ui->setupUi(this);
  reload();
}

ObjectWindow::~ObjectWindow() { delete ui; }

void ObjectWindow::reload() {
  this->setWindowTitle(
      QString::number(this->object->getId()) + " (" +
      QString::fromStdString(this->object->getClass()->getName()) + ")");
  ui->idLabel->setText(QString::number(this->object->getId()));
  reloadClasses();
  reloadAttributes();
}

shared_ptr<AnnotatorLib::Object> ObjectWindow::getObject() { return object; }

void ObjectWindow::reloadClasses() {
  ui->objectClassComboBox->clear();
  for (auto &pair : session->getClasses()) {
    ui->objectClassComboBox->addItem(
        QString::fromStdString(pair.second->getName()));
  }
  ui->objectClassComboBox->setCurrentText(
      QString::fromStdString(this->object->getClass()->getName()));
}

void ObjectWindow::reloadAttributes() {
  ui->attributesListWidget->clear();
  for (auto attribute : this->object->getAttributes()) {
    QListWidgetItem *item = new QListWidgetItem(ui->attributesListWidget);
    AttributeItem *attributeItem = new AttributeItem(attribute);
    item->setSizeHint(attributeItem->minimumSizeHint());
    ui->attributesListWidget->setItemWidget(item, attributeItem);
  }
}

void ObjectWindow::on_editClassesButton_clicked() {
  ClassesDialog dialog(session, this);
  dialog.exec();
  reloadClasses();
}

void ObjectWindow::done(int status) {
  if (QDialog::Accepted == status) {
    shared_ptr<AnnotatorLib::Class> selClass = this->session->getClass(
        ui->objectClassComboBox->currentText().toStdString());
    if (selClass == nullptr) {
      (void)QMessageBox::information(
          this, tr("Error"), tr("No such class registered."), QMessageBox::Ok);
      return;
    }
  }
  QDialog::done(status);
}

void ObjectWindow::on_addAttributeButton_clicked()
{
    NewAttributeDialog dlg(session, object, this);
    dlg.exec();
}
