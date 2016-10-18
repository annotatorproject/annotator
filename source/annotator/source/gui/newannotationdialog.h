#ifndef NEWANNOTATIONDIALOG_H
#define NEWANNOTATIONDIALOG_H

#include <AnnotatorLib/Session.h>
#include <QDialog>
#include <memory>

namespace Ui {
class NewAnnotationDialog;
}

class NewAnnotationDialog : public QDialog {
  Q_OBJECT

 public:
  explicit NewAnnotationDialog(std::shared_ptr<AnnotatorLib::Session> session,
                               unsigned long frame_nmb,
                               shared_ptr<AnnotatorLib::Object> sel_obj =
                                   shared_ptr<AnnotatorLib::Object>(nullptr),
                               QWidget *parent = 0);
  ~NewAnnotationDialog();
  void setDimensions(float x, float y, float w, float h);
  void createAnnotation();

 protected:
  bool checkValues();
  void reloadClasses();

  std::shared_ptr<AnnotatorLib::Session> session;
  const unsigned long frame_nmb = 0;
  const shared_ptr<AnnotatorLib::Object> selected_obj;
  Ui::NewAnnotationDialog *ui;
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;

 private slots:
  void on_radioButtonNewObj_clicked();
  void on_radioButtonSelObj_clicked();
  void done(int r) override;  // called when ok or cancel is clicked
  void on_editClassesButton_clicked();
};

#endif  // NEWANNOTATIONDIALOG_H
