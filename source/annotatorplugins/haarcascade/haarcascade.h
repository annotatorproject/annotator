#ifndef HAARCASCADE_H
#define HAARCASCADE_H

#include <opencv2/objdetect/objdetect.hpp>

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Frame.h>

#include <annotator/plugins/plugin.h>
#include "widget.h"

namespace AnnotatorLib {
class Session;
}

namespace Annotator {
namespace Plugins {

class Haarcascade : public Plugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "annotator.haarcascade" FILE "haarcascade.json")
  Q_INTERFACES(Annotator::Plugin)

 public:
  Haarcascade();
  ~Haarcascade();
  QString getName() override;
  QWidget *getWidget() override;

  bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image) override;
  void setObject(AnnotatorLib::Object *object) override;
  AnnotatorLib::Object *getObject() override;
  void setLastAnnotation(AnnotatorLib::Annotation *annotation) override;
  std::vector<AnnotatorLib::Commands::Command *> getCommands() override;
  void setSession(AnnotatorLib::Session *session) override;

  void loadCascade(std::string cascadeFile);
  void setNewObjects(bool newObjects);
  void setObjectName(std::string name);

 protected:
  AnnotatorLib::Frame *frame = 0;
  AnnotatorLib::Frame *lastFrame = 0;

  AnnotatorLib::Annotation *lastAnnotation;
  AnnotatorLib::Object *object;

  cv::Mat frameImg;
  unsigned int objectNr = 0;
  bool newObjects = false;
  std::string objectNames = "noname";
  std::string cascadeFile = ":/haarcascade/haarcascade_frontalface_default.xml";
  cv::CascadeClassifier cascade;

  AnnotatorLib::Session *session = nullptr;

  Widget widget;
};
}
}

#endif  // HAARCASCADE_H
