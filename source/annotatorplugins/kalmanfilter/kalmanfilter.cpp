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

bool KalmanFilter::setFrame(shared_ptr<Frame> frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

// first call
void KalmanFilter::setObject(shared_ptr<Object> object) {
  this->object = object;
}

shared_ptr<Object> KalmanFilter::getObject() { return object; }

// second call
void KalmanFilter::setLastAnnotation(shared_ptr<Annotation> annotation) {
  if (annotation == nullptr || annotation->getObject() != object) return;
  if (lastAnnotation != nullptr &&
      annotation->getObject() == lastAnnotation->getObject())
    return;
  this->lastAnnotation = annotation;
  lastFrame = this->lastAnnotation->getFrame();
}

std::vector<shared_ptr<Commands::Command> > KalmanFilter::getCommands() {
  std::vector<shared_ptr<Commands::Command> > commands;
  return commands;
}
