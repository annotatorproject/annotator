#ifndef TEMPLATEMATCHER_H
#define TEMPLATEMATCHER_H

#include "widget.h"
#include <annotator/plugins/plugin.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

class TemplateMatcher : public Plugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "annotator.templatematcher" FILE "templatematcher.json")
  Q_INTERFACES(Annotator::Plugin)

public:
  TemplateMatcher();
  ~TemplateMatcher();
  QString getName() override;
  QWidget *getWidget() override;

  bool setFrame(shared_ptr<Frame> frame, cv::Mat image) override;
  void setObject(shared_ptr<Object> object) override;
  shared_ptr<Object> getObject() override;
  void setLastAnnotation(shared_ptr<Annotation> annotation) override;
  std::vector<shared_ptr<Commands::Command>> getCommands() override;

  shared_ptr<Annotation> lastAnnotation = nullptr;
  shared_ptr<Object> object = nullptr;

protected:
  Widget widget;

  shared_ptr<Frame> frame = nullptr;
  shared_ptr<Frame> lastFrame = nullptr;
  cv::Mat frameImg;

  bool initialized = true;
  int match_method = cv::TM_CCORR_NORMED;
  cv::Mat templ;
  cv::Mat result;
  const char *image_window = "Source Image";
  const char *result_window = "Result window";
};
}
}

#endif // TEMPLATEMATCHER_H
