#include "annotationswidget.h"
#include "ui_annotationswidget.h"
#include <QLabel>
#include <AnnotatorLib/Annotation.h>
#include <controller/selectioncontroller.h>

AnnotationsWidget::AnnotationsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AnnotationsWidget) {
  ui->setupUi(this);
  initWidget();
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

void AnnotationsWidget::initWidget() {
  ui->treeWidget->setColumnCount(3);

  QStringList labels;
  labels << "Object (id, count)"
         << "Annotation ID"
         << "Frame";
  ui->treeWidget->setHeaderLabels(labels);

  ui->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void AnnotationsWidget::refreshHoleSession() {
  objectIndexMap.clear();
  ui->treeWidget->clear(); //deletes all childs

  if (session != nullptr) {
    for (auto& pair : session->getObjects()) {
      if (pair.second->getAnnotations().size() > 0)
        addObject(pair.second);
    }
  }
}


///////////////SIGNALS////////////////////

void AnnotationsWidget::on_refreshSession() {
  refreshHoleSession();
}

void AnnotationsWidget::on_annotationAdded(shared_ptr<AnnotatorLib::Annotation> a) {
  int idx = objectIndexMap[a->getObject()->getId()];
  //update annonation counter
  ui->treeWidget->topLevelItem(idx)->setText(0, QString::fromStdString(a->getObject()->getName())
                + " (" + QString::number(a->getObject()->getId()) + ", "
                + QString::number(a->getObject()->getAnnotations().size()) + ")");
  addAnnotation(a, ui->treeWidget->topLevelItem(idx));
}

void AnnotationsWidget::on_annotationRemoved(shared_ptr<AnnotatorLib::Annotation> a) {
  refreshHoleSession(); //TODO: reload object only
}

void AnnotationsWidget::addAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation,
                                      QTreeWidgetItem *item) {
  if (!annotation || annotation->isTemporary()) return;

  QTreeWidgetItem *childItem = new QTreeWidgetItem();
  childItem->setText(0, QString(""));
  childItem->setText(1, QString::number(annotation->getId()));
  childItem->setText(2, QString::number(annotation->getFrame()->getFrameNumber()));
  item->addChild( childItem);
}

void AnnotationsWidget::addObject(shared_ptr<AnnotatorLib::Object> object) {
  if (!object) return;
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
  item->setText(0, QString::fromStdString(object->getName())
                + " (" + QString::number(object->getId()) + ", "
                + QString::number(object->getAnnotations().size()) + ")");
  objectIndexMap[object->getId()] = ui->treeWidget->topLevelItemCount() - 1;
  ui->treeWidget->addTopLevelItem(item);

  for (auto& pair : object->getAnnotations()) {
    addAnnotation(pair.second.lock(), item);
  }
}

void AnnotationsWidget::on_objectRemoved(shared_ptr<AnnotatorLib::Object> object) {
  int idx = objectIndexMap[object->getId()];
  delete ui->treeWidget->topLevelItem(idx);
  objectIndexMap.erase(object->getId());
}

void AnnotationsWidget::on_objectSelected(shared_ptr<AnnotatorLib::Object> object) {
  for(auto item : ui->treeWidget->selectedItems())
    item->setSelected(false);
  if (object) {
    int idx = objectIndexMap[object->getId()];
    ui->treeWidget->topLevelItem(idx)->setSelected(true);
  }
}

void AnnotationsWidget::on_treeWidget_currentItemChanged(
    QTreeWidgetItem *current, QTreeWidgetItem *previous) {

  if( previous) previous->setSelected(false);
  if (!current || !current->parent()) return;

  unsigned long id = current->text(1).toULong();
  unsigned long frame_nmb = current->text(2).toULong();
  SelectionController::instance()->setSelectedObject(session->getAnnotation(id)->getObject());
  SelectionController::instance()->setSelectedFrame(frame_nmb);
}
