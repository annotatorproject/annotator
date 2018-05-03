#include "widget.h"
#include <QFile>
#include <QFileDialog>
#include "haarcascade.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
}

Widget::~Widget() { delete ui; }

void Widget::setHaarcascade(Annotator::Plugins::Haarcascade *haarcascade) {
  this->haarcascade = haarcascade;
  initClasses();
}

void Widget::on_pushButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Select Haarcascade XML"), ".", tr("XML Files (*.xml)"));
  if (QFile::exists(fileName)) {
    this->ui->xmlFileLineEdit->setText(fileName);
    haarcascade->loadCascade(fileName.toStdString());
  }
}

void Widget::initClasses() {
  if (haarcascade && haarcascade->getProject() &&
      haarcascade->getProject()->getSession()) {
    ui->classesComboBox->clear();
    for (auto class_ : haarcascade->getProject()->getSession()->getClasses()) {
      ui->classesComboBox->addItem(QString::fromStdString(class_.first));
    }
  }
}

void Widget::on_classesComboBox_currentTextChanged(const QString &arg1) {
  this->haarcascade->setClass(arg1.toStdString());
}
