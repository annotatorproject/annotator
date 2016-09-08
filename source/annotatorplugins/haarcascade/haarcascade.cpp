#include <QtGui/QPainter>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Session.h>

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

bool Haarcascade::setFrame(shared_ptr<Frame> frame, cv::Mat image) {
  this->lastFrame = this->frame;
  this->frame = frame;
  this->frameImg = image;
  return lastFrame != frame;
}

void Haarcascade::setObject(shared_ptr<Object> object) {
  this->object = object;
}

shared_ptr<Object> Haarcascade::getObject() { return this->object; }

void Haarcascade::setLastAnnotation(shared_ptr<Annotation> annotation) {
  this->lastAnnotation = annotation;
}

std::vector<shared_ptr<Commands::Command> > Haarcascade::getCommands() {
  std::vector<shared_ptr<Commands::Command> > commands;
  if (!this->newObjects) return commands;
  try {
    std::vector<cv::Rect> objects;
    cv::Mat frame_gray;
    cv::cvtColor(frameImg, frame_gray, CV_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);
    cascade.detectMultiScale(frame_gray, objects, 1.1, 2,
                             0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    for (size_t i = 0; i < objects.size(); i++) {
      // std::string name = this->objectNames +
      // std::to_string(this->objectNr++);
      float x = objects[i].x;
      float y = objects[i].y;
      float w = objects[i].width;
      float h = objects[i].height;

      // cv::rectangle( frameImg, objects[i], cv::Scalar(0,0,255), 3, CV_AA );
      shared_ptr<Commands::NewAnnotation> nA =
          std::make_shared<Commands::NewAnnotation>(this->session, this->object, frame, x, y,
                                                    w, h);
      commands.push_back(nA);
    }

    // cv::imshow( "Debug Window", frameImg );
  } catch (std::exception &e) {
  }

  return commands;
}

void Haarcascade::setSession(Session *session) {
  this->session = session;
}

void Haarcascade::calculate(shared_ptr<Object> object,
                            shared_ptr<Frame> frame, cv::Mat image) {
  setObject(object);
  setFrame(frame, image);
  for (shared_ptr<Commands::Command> command : getCommands()) {
    session->execute(command);
  }
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
