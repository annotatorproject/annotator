#include "objectswidget.h"
#include "ui_objectswidget.h"
#include <geomObject/annotationgraphicsitem.h>
#include <AnnotatorLib/Session.h>

#include "objectitem.h"

ObjectsWidget::ObjectsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectsWidget)
{
    ui->setupUi(this);
}

ObjectsWidget::~ObjectsWidget()
{
    delete ui;
}

void ObjectsWidget::setSession(AnnotatorLib::Session *session)
{
    this->session = session;
}

void ObjectsWidget::reload()
{
    ui->listWidget->clear();
    for(auto& pair: session->getObjects())
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        ObjectItem *objectItem = new ObjectItem(pair.second);
        item->setSizeHint(objectItem->minimumSizeHint());
        ui->listWidget->setItemWidget(item, objectItem);
    }
    if (AnnotationGraphicsItem::getSelectedAnnotation())
      selectObject(AnnotationGraphicsItem::getSelectedAnnotation()->getObject());
}

void ObjectsWidget::selectObject(shared_ptr<AnnotatorLib::Object> object)
{
    //if nothing is selected deselect all
    if (!object) {
        ui->listWidget->clearSelection();
        ui->listWidget->clearFocus();
      return;
    }
    for(int i = 0; i < ui->listWidget->count(); ++i){
        ObjectItem * oi = (ObjectItem*) ui->listWidget->itemWidget(ui->listWidget->item(i));
        if(oi->getObject() == object){
            ui->listWidget->setCurrentItem(ui->listWidget->item(i));
        }
    }
}

void ObjectsWidget::on_listWidget_itemSelectionChanged()
{
    ObjectItem * selectedItem = (ObjectItem*)ui->listWidget->currentItem();
    //TODO: here is a bug
    //if( selectedItem != nullptr && selectedItem->getObject().get() != nullptr )
    //    emit objectSelected(selectedItem->getObject());
}
