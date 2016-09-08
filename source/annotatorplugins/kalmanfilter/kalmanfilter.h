#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

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

class KalmanFilter : public Plugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "annotator.kalmanfilter" FILE "kalmanfilter.json")
  Q_INTERFACES(Annotator::Plugin)

 public:
  KalmanFilter();
  ~KalmanFilter();
  QString getName() override;
  QWidget *getWidget() override;

  bool setFrame(shared_ptr<Frame> frame, cv::Mat image) override;
  void setObject(shared_ptr<Object> object) override;
  shared_ptr<Object> getObject() override;
  void setLastAnnotation(shared_ptr<Annotation> annotation) override;
  std::vector<shared_ptr<Commands::Command> > getCommands() override;
  void setSession(Session *session) override;
  void calculate(shared_ptr<Object> object, shared_ptr<Frame> frame,
                 cv::Mat image);

 protected:
  shared_ptr<Annotation> lastAnnotation = nullptr;
  shared_ptr<Object> object = nullptr;
  Session *session = nullptr;

  Widget widget;

  shared_ptr<Frame> frame = nullptr;
  shared_ptr<Frame> lastFrame = nullptr;
  cv::Mat frameImg;
};
}
}

#endif  // KALMANFILTER_H
