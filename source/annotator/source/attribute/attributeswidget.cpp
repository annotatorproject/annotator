#include "attributeswidget.h"
#include "ui_attributeswidget.h"
#include "attribute/attributeitem.h"

AttributesWidget::AttributesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttributesWidget)
{
    ui->setupUi(this);
}

AttributesWidget::~AttributesWidget()
{
    delete ui;
}

void AttributesWidget::setSession(AnnotatorLib::Session *session)
{
    this->session = session;
}

void AttributesWidget::reload()
{
    ui->listWidget->clear();
    for(AnnotatorLib::Attribute *attribute: session->getAttributes())
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        AttributeItem *attributeItem = new AttributeItem(attribute);
        item->setSizeHint(attributeItem->minimumSizeHint());
        ui->listWidget->setItemWidget(item, attributeItem);
    }
}
