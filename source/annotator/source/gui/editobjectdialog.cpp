#include <QMessageBox>
#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include "classesdialog.h"
#include <AnnotatorLib/Commands/UpdateObject.h>
#include "controller/commandcontroller.h"


EditObjectDialog::EditObjectDialog(AnnotatorLib::Session *session,
                                 AnnotatorLib::Object* obj,
                                 QWidget *parent)
    : QDialog(parent), session(session), obj(obj), ui(new Ui::EditObjectDialog) {

  ui->setupUi(this);
  ui->objectIdLineEdit->setText(QString::number(this->obj->getId()));

  reloadClasses();
  int index_of_class = ui->objectClassComboBox->findText(QString::fromStdString(obj->getClass()->getName()));
  if ( index_of_class != -1 ) {
     ui->objectClassComboBox->setCurrentIndex(index_of_class);
  }
}

EditObjectDialog::~EditObjectDialog() { delete ui; }

void EditObjectDialog::updateObject() {

  AnnotatorLib::Class * selClass = this->session->getClass(ui->objectClassComboBox->currentText().toStdString());
  if(selClass == nullptr){
      (void) QMessageBox::information(this, tr("Error"), tr("No such class registered."), QMessageBox::Ok);
      return;
  }
  if(selClass == obj->getClass())
    return;

  AnnotatorLib::Commands::UpdateObject* uO = new AnnotatorLib::Commands::UpdateObject(obj, selClass);
  CommandController::instance()->execute(uO);
}

void EditObjectDialog::reloadClasses()
{
    ui->objectClassComboBox->clear();
    for(AnnotatorLib::Class * c: session->getClasses()){
        ui->objectClassComboBox->addItem(QString::fromStdString(c->getName()));
    }
}

void EditObjectDialog::done(int r) {

  if(QDialog::Accepted == r) {
    if(!checkValues()) {
      return;
    }
    updateObject();
  }
  QDialog::done(r);
}

bool EditObjectDialog::checkValues() {

  if (ui->objectClassComboBox->currentText().isEmpty()) {
    (void) QMessageBox::information(this, tr("No Class selected"),
        tr("Please supply a class for the object."), QMessageBox::Ok);
    return false;
  }
  return true;
}

void EditObjectDialog::on_editClassesButton_clicked()
{
    ClassesDialog dialog(session, this);
    dialog.exec();
    reloadClasses();
}



