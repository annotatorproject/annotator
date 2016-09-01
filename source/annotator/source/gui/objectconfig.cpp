#include "objectconfig.h"
#include "ui_objectconfig.h"

ObjectConfig::ObjectConfig(QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectConfig) {
  ui->setupUi(this);
}

ObjectConfig::~ObjectConfig() { delete ui; }
