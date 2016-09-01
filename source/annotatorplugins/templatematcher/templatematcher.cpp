#include "templatematcher.h"
#include "widget.h"

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <QtGui/QPainter>
#include <QDebug>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace Annotator::Plugins;

Annotator::Plugins::TemplateMatcher::TemplateMatcher()
{

}

TemplateMatcher::~TemplateMatcher()
{

}

QString TemplateMatcher::getName()
{
    return "TemplateMatcher";
}

QWidget *TemplateMatcher::getWidget()
{
    return &widget;
}

bool TemplateMatcher::setFrame(AnnotatorLib::Frame *frame, cv::Mat image)
{
    this->lastFrame = this->frame;
    this->frame = frame;
    this->frameImg = image;
    return lastFrame != frame;
}


// first call
void TemplateMatcher::setObject(AnnotatorLib::Object *object)
{
    this->object = object;
    this->initialized = false;
}

AnnotatorLib::Object *TemplateMatcher::getObject()
{
    return object;
}

// second call
void TemplateMatcher::setLastAnnotation(AnnotatorLib::Annotation *annotation)
{
    if(annotation == nullptr || annotation->getObject() != object)
        return;
    if(lastAnnotation != nullptr &&annotation->getObject() == lastAnnotation->getObject())
        return;
    this->lastAnnotation = annotation;
    lastFrame = this->lastAnnotation->getFrame();
    if(!initialized)
    {
        cv::Rect roi(annotation->getX() - annotation->getHRadius() *0.5f,
                     annotation->getY() - annotation->getVRadius() *0.5f,
                     annotation->getHRadius(), annotation->getVRadius());
        this->templ = this->frameImg(roi);
        initialized = true;
    }
}

std::vector<AnnotatorLib::Commands::Command *> TemplateMatcher::getCommands()
{
    std::vector<AnnotatorLib::Commands::Command *> commands;
    if(object == nullptr || frame == nullptr || lastFrame == nullptr || this->lastAnnotation == nullptr)
        return commands;


    try
    {
        cv::Mat img_display;
        frameImg.copyTo( img_display );

        /// Create the result matrix
        int result_cols =  frameImg.cols - templ.cols + 1;
        int result_rows = frameImg.rows - templ.rows + 1;

        result.create( result_rows, result_cols, CV_32FC1 );

        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;

        cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED )
        { matchLoc = minLoc; }
        else
        { matchLoc = maxLoc; }

        int hradius = templ.cols * 0.5;
        int vradius = templ.rows * 0.5;

        int x = matchLoc.x + hradius;
        int y = matchLoc.y + vradius;

        AnnotatorLib::Commands::NewAnnotation * nA =
                new AnnotatorLib::Commands::NewAnnotation(lastAnnotation->getObject(), this->frame,
                                                          x, y, hradius, vradius,
                                                          this->session, false);
        commands.push_back(nA);

        /// Show me what you got
          cv::rectangle( img_display, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
          cv::rectangle( result, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );

          cv::imshow( image_window, img_display );
          cv::imshow( result_window, result );


    }catch(std::exception &e){
        qDebug() << e.what();
    }

    return commands;

}

void TemplateMatcher::setSession(AnnotatorLib::Session *session)
{
    this->session = session;
}

void Annotator::Plugins::TemplateMatcher::calculate(AnnotatorLib::Object *object, AnnotatorLib::Frame *frame, cv::Mat image)
{

}
