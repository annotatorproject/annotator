#include "newobjectdialog.h"
#include "ui_newobjectdialog.h"
#include "classesdialog.h"

#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/NewObject.h>
#include "plugins/pluginloader.h"

NewObjectDialog::NewObjectDialog(AnnotatorLib::Session *session, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewObjectDialog), session(session) {
  ui->setupUi(this);
  reloadClasses();
}

NewObjectDialog::~NewObjectDialog() { delete ui; }

void NewObjectDialog::setDimenstions(float x, float y, float w, float h) {
  this->x = x;
  this->y = y;
  this->w = w;
  this->h = h;
  ui->positionLabel->setText("Position: " + QString::number(x) + "x" +
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
  AnnotatorLib::Commands::NewObject *nO = new AnnotatorLib::Commands::NewObject(
      ui->objectNameLineEdit->text().toStdString(), session);
  session->execute(nO);

  AnnotatorLib::Frame *frame = session->getFrame(this->frame);

  AnnotatorLib::Commands::NewAnnotation *nA =
      new AnnotatorLib::Commands::NewAnnotation(
          nO->getObject(), frame, x, y, w, h, this->session, false);

  session->execute(nA);

  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  plugin->setObject(nO->getObject());
  plugin->setLastAnnotation(nA->getAnnotation());
}

void NewObjectDialog::reloadClasses()
{
    ui->objectClassComboBox->clear();
    for(AnnotatorLib::Class * c: session->getClasses()){
        ui->objectClassComboBox->addItem(QString::fromStdString(c->getName()));
    }
}

void NewObjectDialog::on_buttonBox_accepted() { createObject(); }

void NewObjectDialog::on_editClassesButton_clicked()
{
    ClassesDialog dialog(session, this);
    dialog.exec();
    reloadClasses();
}
