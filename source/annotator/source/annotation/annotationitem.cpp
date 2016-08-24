#include "annotationitem.h"
#include "ui_annotationitem.h"

#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Object.h>

AnnotationItem::AnnotationItem(AnnotatorLib::Annotation *annotation,
                               QWidget *parent)
    : QWidget(parent), ui(new Ui::AnnotationItem), annotation(annotation) {
  ui->setupUi(this);
  reload();

  // TODO: redesign widget

  ui->gridLayout->removeWidget(ui->objectNameLabel);
  ui->gridLayout->removeWidget(ui->positionLabel);
  ui->objectNameLabel->setVisible(false);
  ui->positionLabel->setVisible(false);
  ui->gridLayout->update();
}

AnnotationItem::~AnnotationItem() { delete ui; }

void AnnotationItem::reload() {
  this->ui->idLabel->setText(QString::number(this->annotation->getId()));
  if (this->annotation->getObject()->getClass())
    this->ui->objectNameLabel->setText(QString::fromStdString(
        this->annotation->getObject()->getClass()->getName()));
  this->ui->frameSpinBox->setValue(this->annotation->getFrame()->getNumber());
  this->ui->positionLabel->setText(QString::number(this->annotation->getX()) +
                                   "x" +
                                   QString::number(this->annotation->getY()));
}

AnnotatorLib::Annotation *AnnotationItem::getAnnotation() {
  return this->annotation;
}
