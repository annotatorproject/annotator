#include "correlationtracker.h"
#include "widget.h"

#include <ctype.h>
#include <iostream>
#include <utility>

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Session.h>
#include <dlib/opencv.h>
#include <QDebug>
#include <QtGui/QPainter>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

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

shared_ptr<Object> CorrelationTracker::getObject() const { return object; }

// second call
void CorrelationTracker::setLastAnnotation(shared_ptr<Annotation> annotation) {
  if (!annotation || annotation->getObject() != object) return;

  if (trackerStarted &&
      annotation->getObject() == lastAnnotation->getObject() &&
      *annotation > *lastAnnotation)
    return;

  this->lastAnnotation = annotation;
  lastFrame = this->lastAnnotation->getFrame();

  selection = cv::Rect(lastAnnotation->getX(), lastAnnotation->getY(),
                       lastAnnotation->getWidth(), lastAnnotation->getHeight());

  if (this->frameImg.empty()) {
    setFrame(std::make_shared<AnnotatorLib::Frame>(
                 annotation->getFrame()->getFrameNumber()),
             project->getImageSet()->getImage(
                 annotation->getFrame()->getFrameNumber()));
  }
  dlib::cv_image<dlib::bgr_pixel> cvimg(this->frameImg);
  tracker.start_track(cvimg, dlib::rectangle(selection.x, selection.y,
                                             selection.x + selection.width,
                                             selection.y + selection.height));

  trackerStarted = true;
}

std::vector<shared_ptr<Commands::Command>> CorrelationTracker::getCommands() {
  std::vector<shared_ptr<Commands::Command>> commands;
  if (object == nullptr || frame == nullptr || lastFrame == nullptr ||
      this->lastAnnotation == nullptr || lastFrame == frame)
    return commands;

  try {
    auto res = findObject();

    if (res.first.width > 0) {
      int w = res.first.width;
      int h = res.first.height;

      int x = res.first.x;
      int y = res.first.y;

      shared_ptr<Commands::NewAnnotation> nA =
          std::make_shared<Commands::NewAnnotation>(
              project->getSession(), lastAnnotation->getObject(), this->frame,
              x, y, w, h, std::min(res.second, 0.9f));
      commands.push_back(nA);
    }
  } catch (std::exception &e) {
    qDebug() << e.what();
  }

  return commands;
}

std::pair<cv::Rect, float> CorrelationTracker::findObject() {
  cv::Rect cvrect;
  auto ret = std::make_pair(cvrect, 0.0f);
  if (trackerStarted) {
    dlib::cv_image<dlib::bgr_pixel> cvimg(this->frameImg);
    try {
      ret.second = tracker.update(cvimg);  // returns the peak to side-lobe
                                           // ratio.  This is a number that
                                           // measures how
      dlib::rectangle found = tracker.get_position();
      ret.first =
          cv::Rect(found.left(), found.top(), found.width(), found.height());
    } catch (std::exception &e) {
    }
  }
  return ret;
}
