// Copyright 2017 Annotator Team
#include "annotationwindow.h"
#include "ui_annotationwindow.h"

#include "attribute/attributeitem.h"
#include "attribute/newattributedialog.h"
#include "controller/commandcontroller.h"
#include "object/objectwindow.h"

#include <AnnotatorLib/Attribute.h>
#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Commands/NewAttribute.h>

AnnotationWindow::AnnotationWindow(
    std::shared_ptr<AnnotatorLib::Session> session,
    shared_ptr<AnnotatorLib::Annotation> annotation, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::AnnotationWindow),
      session(session),
      annotation(annotation) {
  ui->setupUi(this);
  reload();
}

AnnotationWindow::~AnnotationWindow() { delete ui; }

void AnnotationWindow::reload()
{
    this->setWindowTitle(
        QString::number(this->annotation->getId()) + " (" +
        QString::fromStdString(this->annotation->getObject()->getClass()->getName()) + ")["+
                QString::number(this->annotation->getFrame()->getFrameNumber())+"]");
    ui->idLabel->setText(QString::number(this->annotation->getId()));
    reloadAttributes();
}

void AnnotationWindow::reloadAttributes() {
  ui->attributesListWidget->clear();
  for (auto attribute : this->annotation->getAttributes()) {
    QListWidgetItem *item = new QListWidgetItem(ui->attributesListWidget);
    AttributeItem *attributeItem = new AttributeItem(attribute);
    item->setSizeHint(attributeItem->minimumSizeHint());
    ui->attributesListWidget->setItemWidget(item, attributeItem);
  }
}

void AnnotationWindow::on_editObjectButton_clicked()
{
    ObjectWindow wnd(session, annotation->getObject());
    wnd.exec();
    reload();
}
