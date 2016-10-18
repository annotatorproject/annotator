#include "attributeswidget.h"
#include "attribute/attributeitem.h"
#include "ui_attributeswidget.h"

AttributesWidget::AttributesWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AttributesWidget) {
  ui->setupUi(this);
}

AttributesWidget::~AttributesWidget() { delete ui; }

void AttributesWidget::setSession(
    std::shared_ptr<AnnotatorLib::Session> session) {
  this->session = session;
}

void AttributesWidget::reload() {
  ui->listWidget->clear();
  for (auto &pair : session->getAttributes()) {
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

    AttributeItem *attributeItem = new AttributeItem(pair.second);
    item->setSizeHint(attributeItem->minimumSizeHint());
    ui->listWidget->setItemWidget(item, attributeItem);
  }
}
