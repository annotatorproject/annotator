#ifndef RemoveRangeDialog_H
#define RemoveRangeDialog_H

#include <AnnotatorLib/Commands/RemoveAnnotationRange.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Project.h>
#include <QDialog>

namespace Ui {
class RemoveRangeDialog;
}

class RemoveRangeDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RemoveRangeDialog(std::shared_ptr<AnnotatorLib::Project> project,
                             std::shared_ptr<AnnotatorLib::Object> object,
                             unsigned long currentFrame, QWidget *parent = 0);
  ~RemoveRangeDialog();

  std::shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange> getCommand();

 protected:
  void updateStartSliderMinMax();
  void updateEndSliderMinMax();
  QPixmap getImgCrop(long frame, int size);

 private slots:

  void on_startFrameSpinBox_valueChanged(int arg1);

  void on_endFrameSpinBox_valueChanged(int arg1);

  void on_startButton_clicked();

 private:
  std::shared_ptr<AnnotatorLib::Project> project;
  std::shared_ptr<AnnotatorLib::Object> object;
  Ui::RemoveRangeDialog *ui;
};

#endif  // RemoveRangeDialog_H
