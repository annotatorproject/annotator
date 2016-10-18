#include "selectedobject.h"
#include <AnnotatorLib/Class.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "controller/selectioncontroller.h"
#include "ui_selectedobject.h"

SelectedObject::SelectedObject(QWidget *parent)
    : QWidget(parent), ui(new Ui::SelectedObject) {
  ui->setupUi(this);
  update(nullptr);
}

SelectedObject::~SelectedObject() { delete ui; }

void SelectedObject::update(std::shared_ptr<AnnotatorLib::Object> object) {
  this->object = object;
  if (object) {
    ui->objectIdLabel->setText(QString::fromStdString(object->getName()));
  } else {
    ui->objectIdLabel->setText(QString::fromStdString("Nothing selected."));
  }

  updateImage();
}

void SelectedObject::updateImage() {
  if (object && object->getFirstAnnotation()) {
    ui->commandLinkButtonFirst->setIcon(getImgCrop(
        object->getFirstAnnotation(), ui->commandLinkButtonFirst->height()));
    ui->commandLinkButtonFirst->show();
  } else {
    ui->commandLinkButtonFirst->setIcon(QIcon());
    ui->commandLinkButtonFirst->hide();
  }
  if (object && object->getLastAnnotation()) {
    ui->commandLinkButtonLast->setIcon(getImgCrop(
        object->getLastAnnotation(), ui->commandLinkButtonLast->height()));
    ui->commandLinkButtonLast->show();
  } else {
    ui->commandLinkButtonLast->setIcon(QIcon());
    ui->commandLinkButtonLast->hide();
  }
}

void SelectedObject::setProject(
    std::shared_ptr<AnnotatorLib::Project> project) {
  this->project = project;
}

void SelectedObject::on_objectSelected(
    shared_ptr<AnnotatorLib::Object> object) {
  update(object);
}

void SelectedObject::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  ui->commandLinkButtonFirst->setIconSize(
      QSize(ui->commandLinkButtonFirst->height(),
            ui->commandLinkButtonFirst->height()));
  ui->commandLinkButtonLast->setIconSize(
      QSize(ui->commandLinkButtonLast->height(),
            ui->commandLinkButtonLast->height()));
}

QPixmap SelectedObject::getImgCrop(
    shared_ptr<AnnotatorLib::Annotation> annotation, int size) const {
  // TODO: getImage changes position, this is an unexpected behaviour...
  // START ugly hack
  // long prev_pos = project->getImageSet()->getPosition();
  cv::Mat tmp = project->getImageSet()->getImage(
      annotation->getFrame()->getFrameNumber());
  // project->getImageSet()->gotoPosition(prev_pos);
  // END ugly hack
  cv::Rect rect(annotation->getX(), annotation->getY(), annotation->getWidth(),
                annotation->getHeight());

  cv::Mat cropped;
  tmp(rect).copyTo(cropped);

  cropped.convertTo(cropped, CV_8U);
  cv::cvtColor(cropped, cropped, CV_BGR2RGB);

  QImage img((const unsigned char *)(cropped.data), cropped.cols, cropped.rows,
             cropped.step, QImage::Format_RGB888);

  QPixmap pim = QPixmap::fromImage(img);
  pim = pim.scaledToHeight(size);
  return pim;
}

void SelectedObject::on_commandLinkButtonFirst_clicked() {
  SelectionController::instance()->setSelectedFrame(
      object->getFirstAnnotation()->getFrame());
}

void SelectedObject::on_commandLinkButtonLast_clicked() {
  SelectionController::instance()->setSelectedFrame(
      object->getLastAnnotation()->getFrame());
}
