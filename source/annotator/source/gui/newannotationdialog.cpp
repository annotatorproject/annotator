// Copyright 2016 Annotator Team
#include "newannotationdialog.h"
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <QMessageBox>
#include "classesdialog.h"
#include "controller/commandcontroller.h"
#include "geomObject/annotationgraphicsitem.h"
#include "plugins/pluginloader.h"
#include "ui_newannotationdialog.h"

NewAnnotationDialog::NewAnnotationDialog(
    std::shared_ptr<AnnotatorLib::Session> session, unsigned long frame_nmb,
    shared_ptr<AnnotatorLib::Object> selected_obj, QWidget* parent)
    : QDialog(parent),
      session(session),
      frame_nmb(frame_nmb),
      selected_obj(selected_obj),
      ui(new Ui::NewAnnotationDialog) {
  ui->setupUi(this);
  if (!selected_obj || session->getObject(selected_obj->getId()) != nullptr) {
    ui->radioButtonSelObj->setEnabled(false);
    ui->radioButtonSelObj->setChecked(false);
    ui->radioButtonNewObj->setChecked(true);
    ui->radioButtonNewObj->setEnabled(false);
  }
  if (ui->radioButtonNewObj->isChecked()) {
    this->on_radioButtonNewObj_clicked();
  } else if (ui->radioButtonSelObj->isChecked()) {
    this->on_radioButtonSelObj_clicked();
  }
  ui->frameSpinBox->setValue(frame_nmb);
  reloadClasses();
}

NewAnnotationDialog::~NewAnnotationDialog() { delete ui; }

void NewAnnotationDialog::setDimensions(float x, float y, float w, float h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  ui->positionValue->setText(QString::number(w) + " x " + QString::number(h) +
                             " @ " + QString::number(x) + " | " +
                             QString::number(y));
}

void NewAnnotationDialog::createAnnotation() {
  shared_ptr<AnnotatorLib::Class> selClass = this->session->getClass(
      ui->objectClassComboBox->currentText().toStdString());
  if (selClass == nullptr) {
    (void)QMessageBox::information(
        this, tr("Error"), tr("No such class registered."), QMessageBox::Ok);
    return;
  }

  shared_ptr<AnnotatorLib::Frame> frame = session->getFrame(this->frame_nmb);
  if (!frame) frame = std::make_shared<AnnotatorLib::Frame>(frame_nmb);
  unsigned long id = (unsigned long)ui->objectIdLineEdit->text().toULong();

  shared_ptr<AnnotatorLib::Commands::NewAnnotation> nA;
  if (this->ui->radioButtonNewObj->isChecked()) {
    nA = std::make_shared<AnnotatorLib::Commands::NewAnnotation>(
        id, selClass, this->session, frame, x, y, w, h);
  } else {
    nA = std::make_shared<AnnotatorLib::Commands::NewAnnotation>(
        this->session, session->getObject(id), frame, x, y, w, h);
  }
  // AnnotationGraphicsItem::setSelectedAnnotation(nA->getAnnotation()); //set
  // the created annotation as selected
  // TODO: emit signal_objectSelection(nA->getAnnotation()->getObject());
  CommandController::instance()->execute(nA);

  //  Annotator::Plugin *plugin =
  //      Annotator::PluginLoader::getInstance().getCurrent();
  //  plugin->setObject(nA->getAnnotation()->getObject());
  //  plugin->setLastAnnotation(nA->getAnnotation());
}

void NewAnnotationDialog::reloadClasses() {
  ui->objectClassComboBox->clear();
  for (auto& pair : session->getClasses()) {
    ui->objectClassComboBox->addItem(
        QString::fromStdString(pair.second->getName()));
  }
}

void NewAnnotationDialog::done(int r) {
  if (QDialog::Accepted == r) {
    if (!checkValues()) {
      return;
    }
    createAnnotation();
  }
  QDialog::done(r);
}

bool NewAnnotationDialog::checkValues() {
  if (ui->objectIdLineEdit->text().isEmpty()) {
    (void)QMessageBox::information(this, tr("No ID"),
                                   tr("Please supply an ID."), QMessageBox::Ok);
    return false;
  }
  if (ui->objectClassComboBox->currentText().isEmpty()) {
    (void)QMessageBox::information(this, tr("No Name"),
                                   tr("Please supply a class for the object."),
                                   QMessageBox::Ok);
    return false;
  }
  return true;
}

void NewAnnotationDialog::on_editClassesButton_clicked() {
  ClassesDialog dialog(session, this);
  dialog.exec();
  reloadClasses();
}

void NewAnnotationDialog::on_radioButtonSelObj_clicked() {
  ui->objectIdLineEdit->setText(QString::number(this->selected_obj->getId()));
  ui->editClassesButton->hide();
  ui->objectClassComboBox->setEnabled(false);
}

void NewAnnotationDialog::on_radioButtonNewObj_clicked() {
  ui->objectIdLineEdit->setText(QString::number(AnnotatorLib::Object::genId()));
  ui->editClassesButton->show();
  ui->objectClassComboBox->setEnabled(true);
}
