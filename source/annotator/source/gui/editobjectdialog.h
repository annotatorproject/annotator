#ifndef EDITOBJDIALOG_H
#define EDITOBJDIALOG_H

#include <AnnotatorLib/Session.h>
#include <QDialog>

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog {
  Q_OBJECT

 public:
  explicit EditObjectDialog(std::shared_ptr<AnnotatorLib::Session> session,
                            shared_ptr<AnnotatorLib::Object> obj,
                            QWidget *parent = 0);
  ~EditObjectDialog();

  void updateObject();

 protected:
  bool checkValues();
  void reloadClasses();

  std::shared_ptr<AnnotatorLib::Session> session;
  shared_ptr<AnnotatorLib::Object> obj;
  Ui::EditObjectDialog *ui;

 private slots:
  void done(int r) override;  // called when ok or cancel is clicked
  void on_editClassesButton_clicked();
};

#endif  // NEWANNOTATIONDIALOG_H
