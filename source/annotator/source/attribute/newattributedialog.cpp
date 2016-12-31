#include "newattributedialog.h"
#include "ui_newattributedialog.h"

NewAttributeDialog::NewAttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAttributeDialog)
{
    ui->setupUi(this);
}

NewAttributeDialog::~NewAttributeDialog()
{
    delete ui;
}
