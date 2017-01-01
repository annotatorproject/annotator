// Copyright 2016-2017 Annotator Team
#ifndef NEWATTRIBUTEDIALOG_H
#define NEWATTRIBUTEDIALOG_H

#include <memory>

#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Session.h>

#include <QDialog>

namespace Ui {
class NewAttributeDialog;
}

class NewAttributeDialog : public QDialog {
  Q_OBJECT

 public:
  explicit NewAttributeDialog(QWidget *parent = 0);
  ~NewAttributeDialog();
  std::shared_ptr<AnnotatorLib::Attribute> getAttribute();

 protected:
  void createAttribute();

 private:
  Ui::NewAttributeDialog *ui;
  void done(int status) override;

  std::shared_ptr<AnnotatorLib::Attribute> attribute;
};

#endif  // NEWATTRIBUTEDIALOG_H
