#include "objectitem.h"
#include "ui_objectitem.h"
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Class.h>

ObjectItem::ObjectItem(AnnotatorLib::Object *object, QWidget *parent) :
    QWidget(parent),
    object(object),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);
    reload();
}

ObjectItem::~ObjectItem()
{
    delete ui;
}

void ObjectItem::reload()
{
    this->ui->idLabel->setText(QString::number(this->object->getId()));
    this->ui->nameLineEdit->setText(QString::fromStdString(this->object->getClass()->getName()));
}

AnnotatorLib::Object *ObjectItem::getObject()
{
    return object;
}
