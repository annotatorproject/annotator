// Copyright 2016-2017 Annotator Team
#include "objectwindow.h"
#include "attribute/attributeitem.h"
#include "attribute/newattributedialog.h"
#include "controller/commandcontroller.h"
#include "gui/classesdialog.h"
#include "ui_objectwindow.h"

#include <assert.h>
#include <memory>

#include <AnnotatorLib/Attribute.h>
#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Commands/NewAttribute.h>
#include <AnnotatorLib/Commands/RemoveAttribute.h>

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

void ObjectWindow::on_addAttributeButton_clicked() {
  NewAttributeDialog dlg(this);
  if (QDialog::Accepted == dlg.exec()) {
    shared_ptr<AnnotatorLib::Commands::NewAttribute> nA;
    nA = std::make_shared<AnnotatorLib::Commands::NewAttribute>(
        session, object, dlg.getAttribute());
    CommandController::instance()->execute(nA);

    reloadAttributes();
  }
}

void ObjectWindow::on_removeAttributeButton_clicked() {
  shared_ptr<AnnotatorLib::Commands::RemoveAttribute> rA;
  std::shared_ptr<AnnotatorLib::Attribute> attr;
  AttributeItem *attributeItem =
      (AttributeItem *)ui->attributesListWidget->itemWidget(
          ui->attributesListWidget->currentItem());
  if (attributeItem) attr = attributeItem->getAttribute();
  if (attr) {
    shared_ptr<AnnotatorLib::Commands::RemoveAttribute> rA =
        std::make_shared<AnnotatorLib::Commands::RemoveAttribute>(session,
                                                                  object, attr);
    CommandController::instance()->execute(rA);

    reloadAttributes();
  }
}
