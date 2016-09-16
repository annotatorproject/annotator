#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

StatisticsDialog::StatisticsDialog(std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent)
    : QDialog(parent), project(project), ui(new Ui::StatisticsDialog) {
  ui->setupUi(this);
  reload();
}

StatisticsDialog::~StatisticsDialog() { delete ui; }

void StatisticsDialog::on_closeButton_clicked() { this->close(); }

void StatisticsDialog::reload() {

  ui->tableWidget->setRowCount(3);
  ui->tableWidget->setColumnCount(3);

  tableHeader << "" << "Count" << "Time per [min]";
  ui->tableWidget->setHorizontalHeaderLabels(tableHeader);

  ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Project"));
  ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Annotation"));

  ui->tableWidget->setItem(1, 1, new QTableWidgetItem(""));
  ui->tableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(project->getSession()->getAnnotations().size())));

  double duration_min = project->getDuration() /60.f;
  ui->tableWidget->setItem(1, 2, new QTableWidgetItem(QString::number(duration_min)));
  ui->tableWidget->setItem(2, 2, new QTableWidgetItem(QString::number(duration_min /(double) project->getSession()->getAnnotations().size())));

  ui->tableWidget->resizeColumnsToContents();
  this->adjustSize();

}

