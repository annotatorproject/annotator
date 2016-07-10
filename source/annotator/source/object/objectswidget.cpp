#include "objectswidget.h"
#include "ui_objectswidget.h"

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
    for(AnnotatorLib::Object *object: session->getObjects())
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        ObjectItem *objectItem = new ObjectItem(object);
        item->setSizeHint(objectItem->minimumSizeHint());
        ui->listWidget->setItemWidget(item, objectItem);
    }
}

void ObjectsWidget::selectObject(AnnotatorLib::Object *object)
{
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
    if(selectedItem != nullptr){
        emit objectSelected(selectedItem->getObject());
    }
}
