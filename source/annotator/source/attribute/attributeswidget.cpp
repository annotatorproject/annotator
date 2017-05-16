// Copyright 2016-2017 Annotator Team
#include "attributeswidget.h"
#include "attribute/attributeitem.h"
#include "attribute/newattributedialog.h"
#include "controller/commandcontroller.h"
#include "ui_attributeswidget.h"

#include <AnnotatorLib/Attribute.h>
#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Commands/NewAttribute.h>
#include <AnnotatorLib/Commands/RemoveAttribute.h>

AttributesWidget::AttributesWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AttributesWidget) {
  ui->setupUi(this);
}

AttributesWidget::~AttributesWidget() { delete ui; }

void AttributesWidget::setSession(
    std::shared_ptr<AnnotatorLib::Session> session) {
    this->session = session;
}

void AttributesWidget::setFrame(long frame_number)
{
    if(session)
    setFrame(session->getFrame(frame_number));
}

void AttributesWidget::setFrame(std::shared_ptr<AnnotatorLib::Frame> frame)
{
 this->frame = frame;
    reload();
}

void AttributesWidget::reload() {
    ui->attributesListWidget->clear();
    if(!frame)
        return;
    for (std::shared_ptr<AnnotatorLib::Attribute> attribute : frame->getAttributes()) {
      QListWidgetItem *item = new QListWidgetItem(ui->attributesListWidget);

      AttributeItem *attributeItem = new AttributeItem(attribute);
      item->setSizeHint(attributeItem->minimumSizeHint());
      ui->attributesListWidget->setItemWidget(item, attributeItem);
    }
}

void AttributesWidget::on_removeAttributeButton_clicked()
{
    shared_ptr<AnnotatorLib::Commands::RemoveAttribute> rA;
    std::shared_ptr<AnnotatorLib::Attribute> attr;
    AttributeItem *attributeItem =
        (AttributeItem *)ui->attributesListWidget->itemWidget(
            ui->attributesListWidget->currentItem());
    if (attributeItem) attr = attributeItem->getAttribute();
    if (attr) {
      shared_ptr<AnnotatorLib::Commands::RemoveAttribute> rA =
          std::make_shared<AnnotatorLib::Commands::RemoveAttribute>(session,
                                                                    frame, attr);
      CommandController::instance()->execute(rA);

      reload();
    }
}

void AttributesWidget::on_addAttributeButton_clicked()
{
    if(!frame) return;
    NewAttributeDialog dlg(this);
    dlg.exec();

    shared_ptr<AnnotatorLib::Commands::NewAttribute> nA;
    nA = std::make_shared<AnnotatorLib::Commands::NewAttribute>(
        session, frame, dlg.getAttribute());
    CommandController::instance()->execute(nA);

    reload();
}
