#ifndef TEMPLATEMATCHER_H
#define TEMPLATEMATCHER_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore/QtPlugin>
#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace AnnotatorLib{
    class Session;
}

namespace Annotator{
namespace Plugins{

class TemplateMatcher : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "annotator.templatematcher" FILE "templatematcher.json")
    Q_INTERFACES(Annotator::Plugin)

public:
    TemplateMatcher();
    ~TemplateMatcher();
    QString getName();
    QWidget *getWidget();

    void addPositive(cv::Mat image);
    void addNegative(cv::Mat image);
    bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image);
    void setObject(AnnotatorLib::Object *object);
    AnnotatorLib::Object *getObject();
    void setLastAnnotation(AnnotatorLib::Annotation *annotation);
    std::vector<AnnotatorLib::Commands::Command *> getCommands();
    AnnotatorLib::Annotation *getAnnotation();
    void setSession(AnnotatorLib::Session * session);

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
    const char* image_window = "Source Image";
    const char* result_window = "Result window";

};

}
}

#endif // TEMPLATEMATCHER_H
