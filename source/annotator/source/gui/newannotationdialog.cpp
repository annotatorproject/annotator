#include <QMessageBox>
#include "newannotationdialog.h"
#include "ui_newannotationdialog.h"
#include "classesdialog.h"
#include "geomObject/annotationgraphicsitem.h"
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include "plugins/pluginloader.h"
#include "controller/commandcontroller.h"


NewAnnotationDialog::NewAnnotationDialog(AnnotatorLib::Session *session,
                                 unsigned long frame_nmb,
                                 AnnotatorLib::Object* selected_obj,
                                 QWidget *parent)
    : QDialog(parent), session(session), frame_nmb(frame_nmb), selected_obj(selected_obj), ui(new Ui::NewAnnotationDialog) {

  ui->setupUi(this);
  if (selected_obj == nullptr || session->getFrame(frame_nmb)->getObject(selected_obj) != nullptr ) {
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
  ui->positionValue->setText(QString::number(w) + " x " +
                             QString::number(h) + " @ " +
                             QString::number(x) + " | " +
                             QString::number(y));
}

void NewAnnotationDialog::createAnnotation() {

  AnnotatorLib::Class * selClass = this->session->getClass(ui->objectClassComboBox->currentText().toStdString());
  if(selClass == nullptr){
      (void) QMessageBox::information(this, tr("Error"),
                                                          tr("No such class registered."), QMessageBox::Ok);
      return;
  }

  AnnotatorLib::Frame *frame = session->getFrame(this->frame_nmb);
  unsigned long id = (unsigned long)ui->objectIdLineEdit->text().toULong();

  AnnotatorLib::Commands::NewAnnotation *nA;
  if (this->ui->radioButtonNewObj->isChecked()) {
    nA = new AnnotatorLib::Commands::NewAnnotation(
            id, selClass, frame, x, y, w, h, this->session, false);
  } else {
    nA = new AnnotatorLib::Commands::NewAnnotation(
              session->getObject(id), frame, x, y, w, h, this->session, false);
  }
  AnnotationGraphicsItem::setSelectedAnnotation(nA->getAnnotation()); //set the created annotation as selected
  //TODO: emit objectSelected(nA->getAnnotation()->getObject());
  CommandController::instance()->execute(nA);

  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  plugin->setObject(nA->getAnnotation()->getObject());
  plugin->setLastAnnotation(nA->getAnnotation());
}

void NewAnnotationDialog::reloadClasses()
{
    ui->objectClassComboBox->clear();
    for(AnnotatorLib::Class * c: session->getClasses()){
        ui->objectClassComboBox->addItem(QString::fromStdString(c->getName()));
    }
}

void NewAnnotationDialog::done(int r) {

  if(QDialog::Accepted == r) {
    if(!checkValues()) {
      return;
    }
    createAnnotation();
  }
  QDialog::done(r);
}

bool NewAnnotationDialog::checkValues() {

  if (ui->objectIdLineEdit->text().isEmpty()) {
    (void) QMessageBox::information(this, tr("No ID"),
        tr("Please supply an ID."), QMessageBox::Ok);
    return false;
  } 
  if (ui->objectClassComboBox->currentText().isEmpty()) {
    (void) QMessageBox::information(this, tr("No Name"),
        tr("Please supply a class for the object."), QMessageBox::Ok);
    return false;
  }
  return true;
}

void NewAnnotationDialog::on_editClassesButton_clicked()
{
    ClassesDialog dialog(session, this);
    dialog.exec();
    reloadClasses();
}


void NewAnnotationDialog::on_radioButtonSelObj_clicked()
{
    ui->objectIdLineEdit->setText(QString::number(this->selected_obj->getId()));
    ui->editClassesButton->hide();
    ui->objectClassComboBox->setEnabled(false);
}


void NewAnnotationDialog::on_radioButtonNewObj_clicked()
{
    ui->objectIdLineEdit->setText(QString::number(AnnotatorLib::Object::genId()));
    ui->editClassesButton->show();
    ui->objectClassComboBox->setEnabled(true);
}

