#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

namespace Annotator {
namespace Plugins {
class Haarcascade;
}
}

class Widget : public QWidget {
  Q_OBJECT

 public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();
  void setHaarcascade(Annotator::Plugins::Haarcascade *haarcascade);

 private slots:
  void on_pushButton_clicked();

  void on_classesComboBox_currentTextChanged(const QString &arg1);

 private:
  Ui::Widget *ui;
  Annotator::Plugins::Haarcascade *haarcascade = nullptr;

  void initClasses();
};

#endif  // WIDGET_H
