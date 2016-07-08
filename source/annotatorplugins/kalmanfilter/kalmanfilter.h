#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <annotator/plugins/plugin.h>
#include "widget.h"

#include <opencv2/core/mat.hpp>

#include <QtCore/QtPlugin>
#include <QtCore/QObject>
#include <QtGui/QIcon>

namespace AnnotatorLib{
    class Session;
}

namespace Annotator{
namespace Plugins{

class KalmanFilter : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "annotator.kalmanfilter" FILE "kalmanfilter.json")
    Q_INTERFACES(Annotator::Plugin)

public:
    KalmanFilter();
    ~KalmanFilter();
    QString getName();
    QWidget *getWidget();

    void addPositive(cv::Mat image);
    void addNegative(cv::Mat image);
    void setFrame(AnnotatorLib::Frame *frame, cv::Mat image);
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


};

}
}

#endif // KALMANFILTER_H
