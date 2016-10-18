#include "objectswidget.h"
#include <AnnotatorLib/Session.h>
#include <controller/selectioncontroller.h>
#include <geomObject/annotationgraphicsitem.h>
#include "ui_objectswidget.h"

ObjectsWidget::ObjectsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ObjectsWidget) {
  ui->setupUi(this);
  ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

ObjectsWidget::~ObjectsWidget() { delete ui; }

void ObjectsWidget::setSession(std::shared_ptr<AnnotatorLib::Session> session) {
  this->session = session;
}

void ObjectsWidget::reload() {
  objectIdToRowMap.clear();
  objectRowToIdMap.clear();
  if (session) {
    ui->listWidget->clear();
    for (auto &pair : session->getObjects()) {
      if (pair.second->getAnnotations().size() > 0) addObject(pair.second);
    }
  }
}

void ObjectsWidget::addObject(shared_ptr<AnnotatorLib::Object> object) {
  QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

  ObjectItem *objectItem = new ObjectItem(object);
  item->setSizeHint(objectItem->minimumSizeHint());
  ui->listWidget->setItemWidget(item, objectItem);
  objectIdToRowMap[object->getId()] = ui->listWidget->count() - 1;
  objectRowToIdMap[ui->listWidget->count() - 1] = object->getId();
}

///////////////////////SIGNALS////////////////////////

void ObjectsWidget::on_refreshSession() { reload(); }

void ObjectsWidget::on_objectAdded(shared_ptr<AnnotatorLib::Object> object) {
  addObject(object);
}

void ObjectsWidget::on_objectRemoved(shared_ptr<AnnotatorLib::Object> object) {
  delete ui->listWidget->item(objectIdToRowMap[object->getId()]);
}

void ObjectsWidget::on_objectSelected(shared_ptr<AnnotatorLib::Object> object) {
  // if nothing is selected deselect all
  if (!object) {
    ui->listWidget->clearSelection();
    ui->listWidget->clearFocus();
    return;
  }
  int row = objectIdToRowMap[object->getId()];
  QListWidgetItem *selected_item = ui->listWidget->item(row);
  ui->listWidget->setCurrentItem(selected_item);
}

// void ObjectsWidget::on_listWidget_itemSelectionChanged()
//{
//  QItemSelectionModel *select = ui->listWidget->selectionModel();
//  if (select->hasSelection()) {
//      int row = select->selectedRows().front().row(); // return selected
//      row(s)
//      unsigned long obj_id = objectRowToIdMap[row];
//      shared_ptr<AnnotatorLib::Object> obj = session->getObject(obj_id);
//      if( obj )
//          SelectionController::instance()->setSelectedObject(obj);
//  }
//}

void ObjectsWidget::on_listWidget_doubleClicked(const QModelIndex &index) {
  int row = index.row();
  unsigned long obj_id = objectRowToIdMap[row];
  shared_ptr<AnnotatorLib::Object> obj = session->getObject(obj_id);
  if (obj) {
    SelectionController::instance()->setSelectedObject(obj);
    SelectionController::instance()->setSelectedFrame(
        obj->getLastAnnotation()->getFrame()->getId());
  }
}
