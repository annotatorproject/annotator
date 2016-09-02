#ifndef CAMSHIFT_H
#define CAMSHIFT_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

#include <opencv2/core/mat.hpp>

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

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

  bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image) override;
  void setObject(AnnotatorLib::Object *object) override;
  AnnotatorLib::Object *getObject() override;
  void setLastAnnotation(AnnotatorLib::Annotation *annotation) override;
  std::vector<AnnotatorLib::Commands::Command *> getCommands() override;
  void setSession(AnnotatorLib::Session *session) override;

  void calculate(AnnotatorLib::Object *object, AnnotatorLib::Frame *frame,
                 cv::Mat image);

 protected:
  cv::Mat frameImg;
  AnnotatorLib::Annotation *lastAnnotation = nullptr;
  AnnotatorLib::Object *object = nullptr;
  AnnotatorLib::Session *session = nullptr;

  bool newSelection = false;
  cv::Rect trackWindow, selection;
  cv::Mat hsv, hist, hue, mask, backproj;

  cv::Rect findObject();

  Widget widget;

  AnnotatorLib::Frame *frame = 0;
  AnnotatorLib::Frame *lastFrame = 0;

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
