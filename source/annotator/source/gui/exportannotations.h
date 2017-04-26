#ifndef EXPORTANNOTATIONS_H
#define EXPORTANNOTATIONS_H

#include <AnnotatorLib/Project.h>

#include <QDialog>

namespace Ui {
class ExportAnnotations;
}

class ExportAnnotations : public QDialog
{
    Q_OBJECT

public:
    explicit ExportAnnotations(std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent = 0);
    ~ExportAnnotations();

private slots:
    void on_destinationButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::ExportAnnotations *ui;
    std::shared_ptr<AnnotatorLib::Project> project;
};

#endif // EXPORTANNOTATIONS_H
