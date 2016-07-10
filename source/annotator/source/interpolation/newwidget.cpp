#include "newwidget.h"
#include "ui_newwidget.h"
#include <QDebug>

NewWidget::NewWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWidget)
{
    ui->setupUi(this);


    this->setWindowFlags(Qt::SplashScreen);            //hide menu bar from dialog
    this->setAttribute(Qt::WA_TranslucentBackground);


    //Initialize ComboBox Status
    QStringList list=(QStringList()<<"--Select Type--"<<"String"<<"Char"<<"Bool"<<"Integer"<<"Double"<<"Float"<<"Long"<<"Short");
    ui->comboBox->addItems(list);
    ui->comboBox->setItemData(0,QColor(Qt::gray), Qt::BackgroundRole);

    QStandardItemModel* model =
            qobject_cast<QStandardItemModel*>(ui->comboBox->model());
    QModelIndex firstIndex = model->index(0, ui->comboBox->modelColumn(),
            ui->comboBox->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    firstItem->setSelectable(false);


}

NewWidget::~NewWidget()
{
    delete ui;
}

/**
 * save	-	emit signal to insert new widget item.
 */
void NewWidget::on_btnPopSave_clicked()
{
    QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
    if (re.exactMatch(ui->lineEdit->text().at(0))){
        QMessageBox::critical(
              this,
              tr("Not valid Input."),
              tr("Attribut name must begin with a character, Not a Number.") );
        return;
    }

    emit setNewWidget(ui->lineEdit->text(),(ui->comboBox->itemData(ui->comboBox->currentIndex())).toString());
    this->close();
}


/**
 * textchanged	-	make new stylsheet if textchanged
 */
void NewWidget::on_lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty()) {
        ui->btnPopSave->setEnabled(true);
        ui->btnPopSave->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #88d, stop: 0.1 #99e, stop: 0.49 #77c, stop: 0.5 #66b, stop: 1 #77c);");
    }else{
        ui->btnPopSave->setEnabled(false);
        ui->btnPopSave->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #1C1C2B, stop: 0.1 #3A3A4E, stop: 0.49 #3A3A46, stop: 0.5 #5F5F77, stop: 1 #808095);");
    }
}


/**
 * close dialog
 */
void NewWidget::on_btnClose_clicked()
{
    this->close();
}
