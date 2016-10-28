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

  std::shared_ptr<AnnotatorLib::Project> getProject();
  QString getStorageType();
  QString getImageSetType();

 private slots:
  void on_pushButtonOk_clicked();
  void on_imageSetPathPushButton_clicked();
  void on_projectFilePushButton_clicked();
  void on_storagePathPushButton_clicked();
  void checkLineEdits();

 private:
  Ui::NewProjectDialog *ui;
  std::shared_ptr<AnnotatorLib::Project> project = nullptr;
  void createFile(std::string path);
};

#endif  // NEWPROJECTDIALOG_H
