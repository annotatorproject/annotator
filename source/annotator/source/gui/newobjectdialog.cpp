#include <QMessageBox>
#include "newobjectdialog.h"
#include "ui_newobjectdialog.h"
#include "classesdialog.h"

#include <AnnotatorLib/Commands/NewAnnotation.h>
#include "plugins/pluginloader.h"
#include "controller/commandcontroller.h"

NewObjectDialog::NewObjectDialog(AnnotatorLib::Session *session, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewObjectDialog), session(session) {

  ui->setupUi(this);
  ui->objectIdLineEdit->setText(QString::number(AnnotatorLib::Object::genId()));
  reloadClasses();
}

NewObjectDialog::~NewObjectDialog() { delete ui; }

void NewObjectDialog::setDimensions(float x, float y, float w, float h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  ui->positionValue->setText(QString::number(w) + " x " +
                             QString::number(h) + " @ " +
                             QString::number(x) + " | " +
                             QString::number(y));
}

void NewObjectDialog::setFrame(int frame) {
  this->frame = frame;
  ui->frameSpinBox->setValue(frame);
}

void NewObjectDialog::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void NewObjectDialog::createObject() {

  AnnotatorLib::Class * selClass = this->session->getClass(ui->objectClassComboBox->currentText().toStdString());
  if(selClass == nullptr){
      (void) QMessageBox::information(this, tr("Error"),
                                                          tr("No such class registered."), QMessageBox::Ok);
      return;
  }

  AnnotatorLib::Frame *frame = session->getFrame(this->frame);
  unsigned long id = (unsigned long)ui->objectIdLineEdit->text().toULong();

  AnnotatorLib::Commands::NewAnnotation *nA =
      new AnnotatorLib::Commands::NewAnnotation(
          id, selClass, frame, x, y, w, h, this->session, false);

  CommandController::instance()->execute(nA);

  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  plugin->setObject(nA->getAnnotation()->getObject());
  plugin->setLastAnnotation(nA->getAnnotation());
}

void NewObjectDialog::reloadClasses()
{
    ui->objectClassComboBox->clear();
    for(AnnotatorLib::Class * c: session->getClasses()){
        ui->objectClassComboBox->addItem(QString::fromStdString(c->getName()));
    }
}

void NewObjectDialog::done(int r) {

  if(QDialog::Accepted == r) {
    if(!checkValues()) {
      return;
    }
    createObject();
  }
  QDialog::done(r);
}

bool NewObjectDialog::checkValues() {

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

void NewObjectDialog::on_editClassesButton_clicked()
{
    ClassesDialog dialog(session, this);
    dialog.exec();
    reloadClasses();
}


void NewObjectDialog::on_radioButtonSelObj_clicked()
{
    //TODO
}

void NewObjectDialog::on_radioButtonNewObj_clicked()
{
    //TODO
}
