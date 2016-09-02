#ifndef TEMPLATEMATCHER_H
#define TEMPLATEMATCHER_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

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

  bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image) override;
  void setObject(AnnotatorLib::Object *object) override;
  AnnotatorLib::Object *getObject() override;
  void setLastAnnotation(AnnotatorLib::Annotation *annotation) override;
  std::vector<AnnotatorLib::Commands::Command *> getCommands() override;
  void setSession(AnnotatorLib::Session *session) override;
  void calculate(AnnotatorLib::Object *object, AnnotatorLib::Frame *frame,
                 cv::Mat image);

  AnnotatorLib::Annotation *lastAnnotation = nullptr;
  AnnotatorLib::Object *object = nullptr;
  AnnotatorLib::Session *session = nullptr;

 protected:
  Widget widget;

  AnnotatorLib::Frame *frame = nullptr;
  AnnotatorLib::Frame *lastFrame = nullptr;
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

#endif  // TEMPLATEMATCHER_H
