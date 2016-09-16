#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <AnnotatorLib/Project.h>
#include <QDialog>

namespace Ui {
class StatisticsDialog;
}

class StatisticsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit StatisticsDialog(std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent = 0);
  ~StatisticsDialog();

 private slots:
  void on_closeButton_clicked();

 protected:

  void reload();
  std::shared_ptr<AnnotatorLib::Project> project;

 private:
  Ui::StatisticsDialog *ui;
  QStringList tableHeader;
};

#endif  // STATSDIALOG_H
