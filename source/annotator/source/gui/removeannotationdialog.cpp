#include "removeannotationdialog.h"
#include "ui_removeannotationdialog.h"


RemoveAnnotationDialog::RemoveAnnotationDialog(std::shared_ptr<AnnotatorLib::Session> session, std::shared_ptr<AnnotatorLib::Object> object, unsigned long currentFrame, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveAnnotationDialog)
{
    this->session = session;
    this->object = object;
    ui->setupUi(this);
    try{
        ui->startSpinBox->setValue(currentFrame);
        ui->startSpinBox->setMinimum(object->getFirstAnnotation()->getFrame()->getFrameNumber());
        ui->endSpinBox->setMinimum(object->getFirstAnnotation()->getFrame()->getFrameNumber());
        ui->startSpinBox->setMaximum(object->getLastAnnotation()->getFrame()->getFrameNumber());
        ui->endSpinBox->setMaximum(object->getLastAnnotation()->getFrame()->getFrameNumber());
        ui->endSpinBox->setValue(object->getLastAnnotation()->getFrame()->getFrameNumber());
    } catch(...){

    }
}


RemoveAnnotationDialog::~RemoveAnnotationDialog()
{
    delete ui;
}

std::shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange> RemoveAnnotationDialog::getCommand()
{
    return shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange>(
                new AnnotatorLib::Commands::RemoveAnnotationRange(session,
                                                                  object,
                                                                  (unsigned long) ui->startSpinBox->value(),
                                                                  (unsigned long) ui->endSpinBox->value()));
}
