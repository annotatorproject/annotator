#ifndef SAVEASDIALOG_H
#define SAVEASDIALOG_H

#include <memory>

#include <QDialog>

#include <AnnotatorLib/Project.h>

namespace Ui {
class SaveAsDialog;
}

class SaveAsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit SaveAsDialog(std::shared_ptr<AnnotatorLib::Project> project,
                        QWidget *parent = 0);
  ~SaveAsDialog();

 private slots:
  void on_destinationButton_clicked();
  void on_buttonBox_accepted();

 private:
  Ui::SaveAsDialog *ui;
  std::shared_ptr<AnnotatorLib::Project> project;
};

#endif  // SAVEASDIALOG_H
