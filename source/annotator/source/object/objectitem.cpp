#include <assert.h>
#include "objectitem.h"
#include "ui_objectitem.h"
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Class.h>

ObjectItem::ObjectItem(shared_ptr<AnnotatorLib::Object> object, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectItem),
    object(object)
{
    assert(object != nullptr);
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
    this->ui->nameLineEdit->setText(QString::fromStdString(this->object->getName()));
}

shared_ptr<AnnotatorLib::Object> ObjectItem::getObject()
{
    return object;
}
