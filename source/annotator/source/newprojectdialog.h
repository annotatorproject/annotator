#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <AnnotatorLib/Project.h>
#include <QDialog>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog {
  Q_OBJECT

 public:
  explicit NewProjectDialog(QWidget *parent = 0);
  ~NewProjectDialog();

  AnnotatorLib::Project *getProject();
  QString getStorageType();
  QString getImageSetType();

 private slots:
  void on_buttonBox_accepted();

  void on_imageSetPathPushButton_clicked();

  void on_projectFilePushButton_clicked();

  void on_storagePathPushButton_clicked();

 private:
  Ui::NewProjectDialog *ui;
  AnnotatorLib::Project *project = nullptr;
};

#endif  // NEWPROJECTDIALOG_H
