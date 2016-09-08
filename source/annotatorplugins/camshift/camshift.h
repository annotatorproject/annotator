#ifndef CAMSHIFT_H
#define CAMSHIFT_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

#include <opencv2/core/mat.hpp>
#include <memory>
#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

using std::shared_ptr;
using namespace AnnotatorLib;

namespace AnnotatorLib {
class Session;
}

namespace Annotator {
namespace Plugins {

class CamShift : public Plugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "annotator.camshift" FILE "camshift.json")
  Q_INTERFACES(Annotator::Plugin)

 public:
  CamShift();
  ~CamShift();
  QString getName() override;
  QWidget *getWidget() override;

  bool setFrame(shared_ptr<Frame> frame, cv::Mat image) override;
  void setObject(shared_ptr<Object> object) override;
  shared_ptr<Object> getObject() override;
  void setLastAnnotation(shared_ptr<Annotation> annotation) override;
  std::vector<shared_ptr<Commands::Command>> getCommands() override;
  void setSession(Session *session) override;

  void calculate(shared_ptr<Object> object, shared_ptr<Frame> frame,
                 cv::Mat image);

 protected:
  cv::Mat frameImg;
  shared_ptr<Annotation> lastAnnotation = nullptr;
  shared_ptr<Object> object = nullptr;
  Session *session = nullptr;

  bool newSelection = false;
  cv::Rect trackWindow, selection;
  cv::Mat hsv, hist, hue, mask, backproj;

  cv::Rect findObject();

  Widget widget;

  shared_ptr<Frame> frame = 0;
  shared_ptr<Frame> lastFrame = 0;

  int vmin = 10;
  int vmax = 256;
  int smin = 30;

  int hsize = 16;
  float hranges[2];
  const float *phranges;
};
}
}

#endif  // CAMSHIFT_H
