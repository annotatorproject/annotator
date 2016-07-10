#include "objectitem.h"
#include "ui_objectitem.h"

ObjectItem::ObjectItem(AnnotatorLib::Object *object, QWidget *parent) :
    QWidget(parent),
    object(object),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);
    reload();

    ui->gridLayout->removeWidget(ui->idLabel);
    ui->idLabel->setVisible(false);
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

AnnotatorLib::Object *ObjectItem::getObject()
{
    return object;
}
