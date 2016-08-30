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
    QString getName() override;
    QWidget *getWidget() override;

    bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image) override;
    void setObject(AnnotatorLib::Object *object) override;
    AnnotatorLib::Object *getObject() override;
    void setLastAnnotation(AnnotatorLib::Annotation *annotation) override;
    std::vector<AnnotatorLib::Commands::Command *> getCommands() override;
    void setSession(AnnotatorLib::Session * session) override;

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
