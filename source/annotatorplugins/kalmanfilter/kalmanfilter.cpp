#include "kalmanfilter.h"
#include "widget.h"

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Session.h>

#include <QDebug>
#include <QtGui/QPainter>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include <ctype.h>
#include <iostream>

using namespace Annotator::Plugins;

Annotator::Plugins::KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

QString KalmanFilter::getName() { return "KalmanFilter"; }

QWidget *KalmanFilter::getWidget() { return &widget; }

bool KalmanFilter::setFrame(AnnotatorLib::Frame *frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

// first call
void KalmanFilter::setObject(AnnotatorLib::Object *object) {
  this->object = object;
}

AnnotatorLib::Object *KalmanFilter::getObject() { return object; }

// second call
void KalmanFilter::setLastAnnotation(AnnotatorLib::Annotation *annotation) {
  if (annotation == nullptr || annotation->getObject() != object) return;
  if (lastAnnotation != nullptr &&
      annotation->getObject() == lastAnnotation->getObject())
    return;
  this->lastAnnotation = annotation;
  lastFrame = this->lastAnnotation->getFrame();
}

std::vector<AnnotatorLib::Commands::Command *> KalmanFilter::getCommands() {
  std::vector<AnnotatorLib::Commands::Command *> commands;
  return commands;
}

void KalmanFilter::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void KalmanFilter::calculate(AnnotatorLib::Object *object,
                             AnnotatorLib::Frame *frame, cv::Mat image) {
  setObject(object);
  setFrame(frame, image);
  for (AnnotatorLib::Commands::Command *command : getCommands()) {
    session->execute(command);
  }
}
