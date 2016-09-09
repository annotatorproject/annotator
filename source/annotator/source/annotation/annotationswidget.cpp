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
  labels << "Object (id, count)"
         << "Annotation ID"
         << "Frame";
  ui->treeWidget->setHeaderLabels(labels);

  for (auto& pair : session->getObjects()) {
    addObject(pair.second);
  }
}

void AnnotationsWidget::addAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation,
                                      QTreeWidgetItem *item) {
  if (!annotation) return;

  AnnotationItem *annotationItem = new AnnotationItem(annotation);
  QTreeWidgetItem *childItem = new QTreeWidgetItem();
  item->addChild(childItem);
  ui->treeWidget->setItemWidget(childItem, 1, annotationItem);
  if (!annotation->isLast()) addAnnotation(annotation->getNext(), item);
}

void AnnotationsWidget::addObject(shared_ptr<AnnotatorLib::Object> object) {
  shared_ptr<AnnotatorLib::Annotation> firstAnnotation = object->getFirstAnnotation();
  if (!firstAnnotation) return;
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
  item->setText(0, QString::fromStdString(object->getName())
                + " (" + QString::number(object->getId()) + ", "
                + QString::number(object->getAnnotations().size()) + ")");
  ui->treeWidget->addTopLevelItem(item);
  addAnnotation(firstAnnotation, item);
}

void AnnotationsWidget::on_treeWidget_currentItemChanged(
    QTreeWidgetItem *current, QTreeWidgetItem *previous) {
  AnnotationItem *item =
      (AnnotationItem *)ui->treeWidget->itemWidget(current, 1);
  if (item != nullptr) {
    emit objectSelected(item->getAnnotation()->getObject());
    if (item->getAnnotation()->getFrame().get() != nullptr)
      emit frameSelected( item->getAnnotation()->getFrame()->getFrameNumber());
  }
}

void AnnotationsWidget::on_refreshButton_clicked() { reload(); }
