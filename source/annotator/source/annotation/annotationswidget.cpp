#include "annotationswidget.h"
#include "ui_annotationswidget.h"

#include <AnnotatorLib/Annotation.h>

#include "annotation/annotationitem.h"

AnnotationsWidget::AnnotationsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnnotationsWidget) {
  ui->setupUi(this);
}

AnnotationsWidget::~AnnotationsWidget() { delete ui; }

void AnnotationsWidget::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void AnnotationsWidget::reload() {
  ui->treeWidget->clear();
  ui->treeWidget->setColumnCount(2);
  ui->treeWidget->setColumnWidth(0, 200);
  QStringList labels;
  labels << "Object"
         << "Annotation ID"
         << "Frame";
  ui->treeWidget->setHeaderLabels(labels);

  for (AnnotatorLib::Object *object : session->getObjects()) {
    addObject(object);
  }
}

void AnnotationsWidget::addAnnotation(AnnotatorLib::Annotation *annotation,
                                      QTreeWidgetItem *item) {
  if (annotation == nullptr) return;

  AnnotationItem *annotationItem = new AnnotationItem(annotation);
  QTreeWidgetItem *childItem = new QTreeWidgetItem();
  item->addChild(childItem);
  ui->treeWidget->setItemWidget(childItem, 1, annotationItem);
  if (!annotation->isLast()) addAnnotation(annotation->getNext(), item);
}

void AnnotationsWidget::addObject(AnnotatorLib::Object *object) {
  AnnotatorLib::Annotation *firstAnnotation = object->getFirstAnnotation();
  if (firstAnnotation == nullptr) return;
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
  item->setText(0, QString::fromStdString(object->getClass()
                                              ? object->getClass()->getName()
                                              : "unnamed") +
                       " (" + QString::number(object->getId()) + ")");
  ui->treeWidget->addTopLevelItem(item);
  addAnnotation(firstAnnotation, item);
}

void AnnotationsWidget::on_treeWidget_currentItemChanged(
    QTreeWidgetItem *current, QTreeWidgetItem *previous) {
  AnnotationItem *item =
      (AnnotationItem *)ui->treeWidget->itemWidget(current, 1);
  if (item != nullptr) {
    emit objectSelected(item->getAnnotation()->getObject());
    if (item->getAnnotation()->getFrame() != nullptr)
      emit frameSelected(
          item->getAnnotation()->getFrame()->getFrameNumber());  // TODO: jumps
                                                                 // to wrong
                                                                 // frame
                                                                 // (shifted by
                                                                 // -1)
  }
}

void AnnotationsWidget::on_refreshButton_clicked() { reload(); }
