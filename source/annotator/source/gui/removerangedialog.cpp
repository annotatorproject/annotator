#include "removerangedialog.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "controller/commandcontroller.h"
#include "ui_removerangedialog.h"

RemoveRangeDialog::RemoveRangeDialog(
    std::shared_ptr<AnnotatorLib::Project> project,
    std::shared_ptr<AnnotatorLib::Object> object, unsigned long currentFrame,
    QWidget *parent)
    : QDialog(parent), ui(new Ui::RemoveRangeDialog) {
  this->object = object;
  this->project = project;
  ui->setupUi(this);

  try {
    ui->startFrameSpinBox->setValue(currentFrame);
    ui->startFrameSpinBox->setMinimum(
        object->getFirstAnnotation()->getFrame()->getFrameNumber());
    ui->endFrameSpinBox->setMinimum(
        object->getFirstAnnotation()->getFrame()->getFrameNumber());
    ui->startFrameSpinBox->setMaximum(
        object->getLastAnnotation()->getFrame()->getFrameNumber());
    ui->endFrameSpinBox->setMaximum(
        object->getLastAnnotation()->getFrame()->getFrameNumber());
    ui->endFrameSpinBox->setValue(
        object->getLastAnnotation()->getFrame()->getFrameNumber());
  } catch (...) {
  }
  updateEndSliderMinMax();
  updateStartSliderMinMax();
}

RemoveRangeDialog::~RemoveRangeDialog() { delete ui; }

QPixmap RemoveRangeDialog::getImgCrop(long frame, int size) {
  // long prev_pos = project->getImageSet()->getPosition();
  cv::Mat tmp = project->getImageSet()->getImage(frame);
  // project->getImageSet()->gotoPosition(prev_pos);

  tmp.convertTo(tmp, CV_8U);
  cv::cvtColor(tmp, tmp, CV_BGR2RGB);

  QImage img((const unsigned char *)(tmp.data), tmp.cols, tmp.rows, tmp.step,
             QImage::Format_RGB888);

  QPixmap pim = QPixmap::fromImage(img);
  pim = pim.scaledToHeight(size);
  return pim;
}

std::shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange>
RemoveRangeDialog::getCommand() {
  return shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange>(
      new AnnotatorLib::Commands::RemoveAnnotationRange(
          project->getSession(), object,
          (unsigned long)ui->startFrameSpinBox->value(),
          (unsigned long)ui->endFrameSpinBox->value()));
}

void RemoveRangeDialog::on_startFrameSpinBox_valueChanged(int arg1) {
  ui->startFrameImageLabel->setPixmap(getImgCrop(arg1, 128));
  updateEndSliderMinMax();
}

void RemoveRangeDialog::on_endFrameSpinBox_valueChanged(int arg1) {
  ui->endFrameImageLabel->setPixmap(getImgCrop(arg1, 128));
  updateStartSliderMinMax();
}

void RemoveRangeDialog::on_startButton_clicked() {
  CommandController::instance()->execute(getCommand());
  this->accept();
}

void RemoveRangeDialog::updateStartSliderMinMax() {
  ui->startFrameSlider->setMaximum(ui->endFrameSlider->value());
  ui->startFrameSpinBox->setMaximum(ui->endFrameSlider->value());
  ui->startFrameSpinBox->setMinimum(
      object->getFirstAnnotation()->getFrame()->getFrameNumber());
  ui->startFrameSlider->setMinimum(
      object->getFirstAnnotation()->getFrame()->getFrameNumber());
}

void RemoveRangeDialog::updateEndSliderMinMax() {
  ui->endFrameSlider->setMaximum(
      object->getLastAnnotation()->getFrame()->getFrameNumber());
  ui->endFrameSpinBox->setMaximum(
      object->getLastAnnotation()->getFrame()->getFrameNumber());
  ui->endFrameSpinBox->setMinimum(ui->startFrameSlider->value());
  ui->endFrameSlider->setMinimum(ui->startFrameSlider->value());
}
