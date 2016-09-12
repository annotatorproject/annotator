#include "annotationswidget.h"
#include "ui_annotationswidget.h"
#include <QLabel>
#include <AnnotatorLib/Annotation.h>

AnnotationsWidget::AnnotationsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnnotationsWidget) {
  ui->setupUi(this);
}

AnnotationsWidget::~AnnotationsWidget() { delete ui; }

void AnnotationsWidget::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void AnnotationsWidget::resizeEvent(QResizeEvent *event) {
  for ( int i = 0; i < ui->treeWidget->columnCount() - 1; ++i)
    ui->treeWidget->setColumnWidth(i, ui->treeWidget->width() / ui->treeWidget->columnCount());
  QWidget::resizeEvent(event);
}

void AnnotationsWidget::reload() {
  ui->treeWidget->clear(); //deletes all childs
  ui->treeWidget->setColumnCount(3);

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
  if (!annotation || annotation->isTemporary()) return;

  QTreeWidgetItem *childItem = new QTreeWidgetItem();
  item->addChild(childItem);
  ui->treeWidget->setItemWidget(childItem, 1, new QLabel(QString::number(annotation->getId())));
  ui->treeWidget->setItemWidget(childItem, 2, new QLabel(QString::number(annotation->getFrame()->getFrameNumber())));

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
  QLabel *item_id =
      (QLabel *)ui->treeWidget->itemWidget(current, 1);
  if (item_id != nullptr) {
    emit signal_objectSelection(session->getAnnotation(item_id->text().toULong())->getObject());
    emit signal_frameSelection( session->getAnnotation(item_id->text().toULong())->getFrame()->getFrameNumber());
  }
}

//TODO
void AnnotationsWidget::on_refreshButton_clicked() { reload(); }
