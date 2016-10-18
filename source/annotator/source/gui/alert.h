#ifndef ALERT_H
#define ALERT_H

#include <QMessageBox>

namespace Ui {
class Alert;
}

class Alert : public QMessageBox {
  Q_OBJECT

 public:
  Alert() : timeout(5), autoClose(false), currentTime(0) {}
  void showEvent(QShowEvent *event);
  void setAutoClose(bool b) { autoClose = b; }
  void setTimeout(int time) { timeout = time; }

 protected:
  void timerEvent(QTimerEvent *event);

  int timeout;
  bool autoClose;
  int currentTime;
};

#endif  // ALERT_H
