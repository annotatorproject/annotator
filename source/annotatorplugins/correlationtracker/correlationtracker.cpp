#include "correlationtracker.h"
#include "widget.h"

#include <QDebug>
#include <QtGui/QPainter>
#include <ctype.h>
#include <dlib/opencv.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Session.h>

using namespace Annotator::Plugins;

CorrelationTracker::CorrelationTracker() {
  hranges[0] = 0.0;
  hranges[1] = 180.0;
  phranges = hranges;
}

CorrelationTracker::~CorrelationTracker() {}

QString CorrelationTracker::getName() { return "CorrelationTracker"; }

QWidget *CorrelationTracker::getWidget() { return &widget; }

bool CorrelationTracker::setFrame(shared_ptr<Frame> frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

// first call
void CorrelationTracker::setObject(shared_ptr<Object> object) {
  this->object = object;
}

shared_ptr<Object> CorrelationTracker::getObject() { return object; }

// second call
void CorrelationTracker::setLastAnnotation(shared_ptr<Annotation> annotation) {
  if (annotation == nullptr || annotation->getObject() != object)
    return;
  if (lastAnnotation != nullptr &&
      annotation->getObject() == lastAnnotation->getObject())
    return;

  this->lastAnnotation = annotation;
  selection = cv::Rect(lastAnnotation->getX(), lastAnnotation->getY(),
                       lastAnnotation->getWidth(), lastAnnotation->getHeight());

  if (this->frameImg.empty())
    return;
  dlib::cv_image<dlib::bgr_pixel> cvimg(this->frameImg);
  tracker.start_track(cvimg, dlib::rectangle(selection.x, selection.y,
                                             selection.x + selection.width,
                                             selection.y + selection.height));

  lastFrame = this->lastAnnotation->getFrame();
  trackerStarted = true;
  newSelection = true;
}

std::vector<shared_ptr<Commands::Command>> CorrelationTracker::getCommands() {
  std::vector<shared_ptr<Commands::Command>> commands;
  if (object == nullptr || frame == nullptr || lastFrame == nullptr ||
      this->lastAnnotation == nullptr || lastFrame == frame)
    return commands;

  try {
    cv::Rect found_rect = findObject();

    int w = found_rect.width;
    int h = found_rect.height;

    int x = found_rect.x;
    int y = found_rect.y;

    shared_ptr<Commands::NewAnnotation> nA =
        std::make_shared<Commands::NewAnnotation>(this->session,
                                                  lastAnnotation->getObject(),
                                                  this->frame, x, y, w, h);
    commands.push_back(nA);

  } catch (std::exception &e) {
    qDebug() << e.what();
  }

  return commands;
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
