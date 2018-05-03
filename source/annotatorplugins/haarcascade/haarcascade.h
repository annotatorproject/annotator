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

using namespace AnnotatorLib;
using std::shared_ptr;

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

  bool setFrame(shared_ptr<Frame> frame, cv::Mat image) override;
  void setObject(shared_ptr<Object> object) override;
  shared_ptr<Object> getObject() const override;
  void setLastAnnotation(shared_ptr<Annotation> annotation) override;
  std::vector<shared_ptr<Commands::Command>> getCommands() override;
  virtual void setProject(
      std::shared_ptr<AnnotatorLib::Project> project) override;
  virtual bool requiresObject() const override { return false; }

  void loadCascade(std::string cascadeFile);
  void setNewObjects(bool newObjects);
  void setClass(std::string name);

  QWidget *getWidget() override;

 protected:
  shared_ptr<Frame> frame = 0;
  shared_ptr<Frame> lastFrame = 0;

  shared_ptr<Annotation> lastAnnotation;
  shared_ptr<Object> object;
  std::string _class;

  cv::Mat frameImg;

  std::string cascadeFile = ":/haarcascade/haarcascade_frontalface_default.xml";
  cv::CascadeClassifier cascade;

  Widget widget;
};
}
}

#endif  // HAARCASCADE_H
