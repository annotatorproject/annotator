// Copyright 2016 Annotator Team
#ifndef OBJECTWINDOW_H
#define OBJECTWINDOW_H

#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Session.h>
#include <QDialog>

#include <memory>

namespace Ui {
class ObjectWindow;
}

class ObjectWindow : public QDialog {
  Q_OBJECT

 public:
  explicit ObjectWindow(std::shared_ptr<AnnotatorLib::Session> session,
                        shared_ptr<AnnotatorLib::Object> object,
                        QWidget *parent = 0);
  ~ObjectWindow();
  void reload();
  shared_ptr<AnnotatorLib::Object> getObject();

 protected:
  void reloadClasses();
  void reloadAttributes();
 private slots:
  void on_editClassesButton_clicked();

  void done(int status) override;  // called when ok or cancel is clicked

  void on_addAttributeButton_clicked();

 private:
  Ui::ObjectWindow *ui;
  const std::shared_ptr<AnnotatorLib::Session> session;
  const std::shared_ptr<AnnotatorLib::Object> object;
};

#endif  // OBJECTWINDOW_H
