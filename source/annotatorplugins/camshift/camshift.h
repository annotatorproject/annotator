#ifndef CAMSHIFT_H
#define CAMSHIFT_H

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

class CamShift : public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "annotator.camshift" FILE "camshift.json")
    Q_INTERFACES(Annotator::Plugin)

public:
    CamShift();
    ~CamShift();
    QString getName();
    QWidget *getWidget();

    void addPositive(cv::Mat image) override;
    void addNegative(cv::Mat image) override;
    bool setFrame(AnnotatorLib::Frame *frame, cv::Mat image) override;
    void setObject(AnnotatorLib::Object *object);
    AnnotatorLib::Object *getObject();
    void setLastAnnotation(AnnotatorLib::Annotation *annotation);
    std::vector<AnnotatorLib::Commands::Command *> getCommands();
    AnnotatorLib::Annotation *getAnnotation();
    void setSession(AnnotatorLib::Session * session);

    cv::Mat frameImg;
    AnnotatorLib::Annotation *lastAnnotation = nullptr;
    AnnotatorLib::Object *object = nullptr;
    AnnotatorLib::Session *session = nullptr;

    bool newSelection = false;
    cv::Rect trackWindow, selection;
    cv::Mat hsv, hist, hue, mask, backproj;

protected:
    cv::Rect findObject();

    Widget widget;

    AnnotatorLib::Frame *frame = 0;
    AnnotatorLib::Frame *lastFrame = 0;

    int vmin = 10;
    int vmax = 256;
    int smin = 30;

    int hsize = 16;
    float hranges[2];
    const float* phranges;


};

}
}

#endif // CAMSHIFT_H
