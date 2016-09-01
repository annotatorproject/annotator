#include "classesdialog.h"
#include "ui_classesdialog.h"

ClassesDialog::ClassesDialog(AnnotatorLib::Session * session, QWidget *parent) :
    QDialog(parent),
    session(session),
    ui(new Ui::ClassesDialog)
{
    ui->setupUi(this);
    reloadClasses();
}

ClassesDialog::~ClassesDialog()
{
    delete ui;
}

void ClassesDialog::on_closeButton_clicked()
{
    this->close();
}

void ClassesDialog::on_addNewButton_clicked()
{
    if(!ui->newLineEdit->text().isEmpty()){
        session->addClass(new AnnotatorLib::Class(ui->newLineEdit->text().toStdString()));
        reloadClasses();
    }
}

void ClassesDialog::reloadClasses()
{
    ui->classesListWidget->clear();
    for(AnnotatorLib::Class *c: session->getClasses()){
        ui->classesListWidget->addItem(QString::fromStdString(c->getName()));
    }
}

void ClassesDialog::on_newLineEdit_returnPressed()
{
    reloadClasses();
}
