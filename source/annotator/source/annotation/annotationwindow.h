// Copyright 2016-2017 Annotator Team
#ifndef ANNOTATIONWINDOW_H
#define ANNOTATIONWINDOW_H

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Session.h>

#include <QDialog>

namespace Ui {
class AnnotationWindow;
}

class AnnotationWindow : public QDialog {
  Q_OBJECT

 public:
  explicit AnnotationWindow(std::shared_ptr<AnnotatorLib::Session> session,
                            shared_ptr<AnnotatorLib::Annotation> annotation,
                            QWidget *parent = 0);
  ~AnnotationWindow();
  void reload();

 protected:
  void reloadAttributes();
 private slots:
  void on_editObjectButton_clicked();

 private:
  Ui::AnnotationWindow *ui;
  const std::shared_ptr<AnnotatorLib::Session> session;
  const std::shared_ptr<AnnotatorLib::Annotation> annotation;
};

#endif  // ANNOTATIONWINDOW_H
