#include "exportannotations.h"
#include "ui_exportannotations.h"

#include <AnnotatorLib/Saver/SaverFactory.h>

#include <QFileDialog>

ExportAnnotations::ExportAnnotations(std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportAnnotations)
{
    this->project = project;
    ui->setupUi(this);
    for(std::string saver: AnnotatorLib::Saver::SaverFactory::availableSaver()){
        ui->fileFormatComboBox->addItem(QString::fromStdString(saver));
    }
}

ExportAnnotations::~ExportAnnotations()
{
    delete ui;
}

void ExportAnnotations::on_destinationButton_clicked()
{
    QString path;
    if(ui->fileFormatComboBox->currentText() == "xml")
        path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                                    tr("Project Files (*.xml)"));
    if(ui->fileFormatComboBox->currentText() == "json")
        path = QFileDialog::getSaveFileName(this, tr("Export Path"), ".",
                                                    tr("Project Files (*.json)"));
    ui->destinationLineEdit->setText(path);
}

void ExportAnnotations::on_buttonBox_accepted()
{
    std::string saverType = ui->fileFormatComboBox->currentText().toStdString();
    AnnotatorLib::Saver::AbstractSaver *saver = AnnotatorLib::Saver::SaverFactory::createSaver(saverType);

    std::string path = ui->destinationLineEdit->text().toStdString();
    saver->setPath(path);
    saver->saveSession(project->getSession().get());
    saver->close();
    delete saver;
}
