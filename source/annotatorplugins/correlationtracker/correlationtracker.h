#ifndef CORRELATIONTRACKER_H
#define CORRELATIONTRACKER_H

#include "widget.h"
#include <annotator/plugins/plugin.h>

#include <dlib/image_processing.h>
#include <opencv2/core/mat.hpp>

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

using namespace AnnotatorLib;
using std::shared_ptr;

namespace AnnotatorLib {
class Session;
}

namespace Annotator {
namespace Plugins {

class CorrelationTracker : public Plugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "annotator.correlationtracker" FILE
                        "correlationtracker.json")
  Q_INTERFACES(Annotator::Plugin)

public:
  CorrelationTracker();
  ~CorrelationTracker();
  QString getName() override;
  QWidget *getWidget() override;

  bool setFrame(shared_ptr<Frame> frame, cv::Mat image) override;
  void setObject(shared_ptr<Object> object) override;
  shared_ptr<Object> getObject() const override;
  void setLastAnnotation(shared_ptr<Annotation> annotation) override;
  std::vector<shared_ptr<Commands::Command>> getCommands() override;

protected:
  cv::Mat frameImg;
  shared_ptr<Annotation> lastAnnotation = nullptr;
  shared_ptr<Object> object = nullptr;

  bool newSelection = false;
  cv::Rect trackWindow, selection;
  cv::Mat hsv, hist, hue, mask, backproj;

  cv::Rect findObject();

  Widget widget;

  shared_ptr<Frame> frame = 0;
  shared_ptr<Frame> lastFrame = 0;

  dlib::correlation_tracker tracker;
  bool trackerStarted = false;

  int vmin = 10;
  int vmax = 256;
  int smin = 30;

  int hsize = 16;
  float hranges[2];
  const float *phranges;
};
}
}

#endif // CORRELATIONTRACKER_H
