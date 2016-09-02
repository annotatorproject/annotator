#include "correlationtracker.h"

#include <ctype.h>
#include <dlib/opencv.h>
#include <QDebug>
#include <QtGui/QPainter>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Session.h>
#include "widget.h"

using namespace Annotator::Plugins;

CorrelationTracker::CorrelationTracker() {
  hranges[0] = 0.0;
  hranges[1] = 180.0;
  phranges = hranges;
}

CorrelationTracker::~CorrelationTracker() {}

QString CorrelationTracker::getName() { return "CorrelationTracker"; }

QWidget *CorrelationTracker::getWidget() { return &widget; }

bool CorrelationTracker::setFrame(AnnotatorLib::Frame *frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

// first call
void CorrelationTracker::setObject(AnnotatorLib::Object *object) {
  this->object = object;
}

AnnotatorLib::Object *CorrelationTracker::getObject() { return object; }

// second call
void CorrelationTracker::setLastAnnotation(
    AnnotatorLib::Annotation *annotation) {
  if (annotation == nullptr || annotation->getObject() != object) return;
  if (lastAnnotation != nullptr &&
      annotation->getObject() == lastAnnotation->getObject())
    return;

  this->lastAnnotation = annotation;
  selection = cv::Rect(lastAnnotation->getX(), lastAnnotation->getY(),
                       lastAnnotation->getWidth(), lastAnnotation->getHeight());

  if (this->frameImg.empty()) return;
  dlib::cv_image<dlib::bgr_pixel> cvimg(this->frameImg);
  tracker.start_track(cvimg, dlib::rectangle(selection.x, selection.y,
                                             selection.x + selection.width,
                                             selection.y + selection.height));

  lastFrame = this->lastAnnotation->getFrame();
  trackerStarted = true;
  newSelection = true;
}

std::vector<AnnotatorLib::Commands::Command *>
CorrelationTracker::getCommands() {
  std::vector<AnnotatorLib::Commands::Command *> commands;
  if (object == nullptr || frame == nullptr || lastFrame == nullptr ||
      this->lastAnnotation == nullptr || lastFrame == frame)
    return commands;

  try {
    cv::Rect found_rect = findObject();

    int w = found_rect.width;
    int h = found_rect.height;

    int x = found_rect.x;
    int y = found_rect.y;

    AnnotatorLib::Commands::NewAnnotation *nA =
        new AnnotatorLib::Commands::NewAnnotation(lastAnnotation->getObject(),
                                                  this->frame, x, y, w, h,
                                                  this->session, false);
    commands.push_back(nA);

  } catch (std::exception &e) {
    qDebug() << e.what();
  }

  return commands;
}

void CorrelationTracker::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void CorrelationTracker::calculate(AnnotatorLib::Object *object,
                                   AnnotatorLib::Frame *frame, cv::Mat image) {
  setObject(object);
  setFrame(frame, image);
  for (AnnotatorLib::Commands::Command *command : getCommands()) {
    session->execute(command);
  }
}

cv::Rect CorrelationTracker::findObject() {
  cv::Rect cvrect;
  if (trackerStarted) {
    dlib::cv_image<dlib::bgr_pixel> cvimg(this->frameImg);
    try {
      tracker.update(cvimg);
      dlib::rectangle found = tracker.get_position();
      cvrect =
          cv::Rect(found.left(), found.top(), found.width(), found.height());
    } catch (std::exception &e) {
    }
  }
  return cvrect;
}
