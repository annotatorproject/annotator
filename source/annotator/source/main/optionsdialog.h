// Copyright 2017 Annotator Team
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit OptionsDialog(QWidget *parent = 0);
  ~OptionsDialog();

 private slots:
  void loadOptions();
  void on_buttonBox_accepted();

  void on_storagesButton_clicked();

 private:
  Ui::OptionsDialog *ui;
};

#endif  // OPTIONSDIALOG_H
