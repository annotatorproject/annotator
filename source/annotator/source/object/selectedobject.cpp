#include "selectedobject.h"
#include "ui_selectedobject.h"

#include <AnnotatorLib/Class.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

SelectedObject::SelectedObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectedObject)
{
    ui->setupUi(this);
}

SelectedObject::~SelectedObject()
{
    delete ui;
}

void SelectedObject::update(std::shared_ptr<AnnotatorLib::Object> object)
{
    this->object = object;
    ui->objectIdLabel->setText(QString::fromStdString(object->getClass()->getName()) + QString::number(object->getId()));
    updateImage();
}

void SelectedObject::updateImage()
{
    if(object.get() && object->getFirstAnnotation().get() && object->getFirstAnnotation()->getFrame().get()){
        std::shared_ptr<AnnotatorLib::Annotation> annotation = object->getFirstAnnotation();
    cv::Mat tmp = project->getImageSet()->getImage(annotation->getFrame()->getFrameNumber());
    cv::Rect rect(annotation->getX(), annotation->getY(), annotation->getWidth(), annotation->getHeight());

    cv::Mat cropped;
    tmp(rect).copyTo(cropped);

    cropped.convertTo(cropped, CV_8U);
    cv::cvtColor(cropped, cropped, CV_BGR2RGB);

    QImage img = QImage((const unsigned char *)(cropped.data), cropped.cols, cropped.rows,
                        cropped.step, QImage::Format_RGB888);
    QPixmap pim = QPixmap::fromImage(img);
    pim.scaledToHeight(64);
    ui->imageLabel->setPixmap(pim);
    } else {
        ui->imageLabel->clear();
    }
}

void SelectedObject::setProject(AnnotatorLib::Project *project)
{
    this->project = project;
}

void SelectedObject::on_objectSelected(shared_ptr<AnnotatorLib::Object> object)
{
    update(object);
}
