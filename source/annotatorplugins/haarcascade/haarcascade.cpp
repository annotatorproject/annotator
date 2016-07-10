#include <QtGui/QPainter>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

#include <AnnotatorLib/Commands/NewAnnotation.h>

#include "haarcascade.h"
#include "widget.h"

using namespace Annotator::Plugins;

Annotator::Plugins::Haarcascade::Haarcascade() {
  widget.setHaarcascade(this);
  loadCascade(cascadeFile);
}

Haarcascade::~Haarcascade() {}

QString Haarcascade::getName() { return "Haarcascade"; }

QWidget *Haarcascade::getWidget() { return &widget; }

void Haarcascade::addPositive(cv::Mat image) {}

void Haarcascade::addNegative(cv::Mat image) {}

void Haarcascade::setFrame(AnnotatorLib::Frame *frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
}

void Haarcascade::setObject(AnnotatorLib::Object *object) {
  this->object = object;
}

AnnotatorLib::Object *Haarcascade::getObject() { return this->object; }

void Haarcascade::setLastAnnotation(AnnotatorLib::Annotation *annotation) {
  this->lastAnnotation = annotation;
}

std::vector<AnnotatorLib::Commands::Command *> Haarcascade::getCommands() {
  std::vector<AnnotatorLib::Commands::Command *> commands;
  if (!this->newObjects) return commands;
  try {
    std::vector<cv::Rect> objects;
    cv::Mat frame_gray;
    cv::cvtColor(frameImg, frame_gray, CV_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);
    cascade.detectMultiScale(frame_gray, objects, 1.1, 2,
                             0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    for (size_t i = 0; i < objects.size(); i++) {
      std::string name = this->objectNames + std::to_string(this->objectNr++);
      float x = objects[i].x;
      float y = objects[i].y;
      float w = objects[i].width;
      float h = objects[i].height;

      // cv::rectangle( frameImg, objects[i], cv::Scalar(0,0,255), 3, CV_AA );
      AnnotatorLib::Commands::NewAnnotation *nA =
          new AnnotatorLib::Commands::NewAnnotation(name, frame, x, y, w, h,
                                                    this->session, true);
      commands.push_back(nA);
    }

    // cv::imshow( "Debug Window", frameImg );
  } catch (std::exception &e) {
  }

  return commands;
}

AnnotatorLib::Annotation *Haarcascade::getAnnotation() {
  AnnotatorLib::Annotation *annotation = nullptr;

  return annotation;
}

void Haarcascade::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void Haarcascade::loadCascade(std::string cascadeFile) {
  this->cascadeFile = cascadeFile;
  try {
    cascade.load(cascadeFile);
  } catch (std::exception &e) {
  }
}

void Haarcascade::setNewObjects(bool newObjects) {
  this->newObjects = newObjects;
}

void Haarcascade::setObjectName(std::string name) {
  this->objectNr = 0;
  this->objectNames = name;
}
