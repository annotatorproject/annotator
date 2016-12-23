#ifndef PLAYER_H
#define PLAYER_H

#include "videoplayer.h"
// Graphics
#include "graphic/owngraphicscene.h"
#include "graphic/owngraphicview.h"

#include <list>
#include <memory>

#include <AnnotatorLib/Project.h>
#include <AnnotatorLib/Session.h>

#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QWidget>

// OPENCV//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using std::shared_ptr;

namespace Ui {
class Player;
}

class AnnotationGraphicsItem;

class Player : public QWidget {
  Q_OBJECT

 public:
  explicit Player(QWidget *parent = 0);
  ~Player();
  void setRateLabel(QLabel *label);
  void closeProject();
  void setProject(std::shared_ptr<AnnotatorLib::Project> project);
  bool LoadFile(const QString &fileName);
  void play();
  void pause();
  Videoplayer *videoplayer;  // videoplayer instance

  OwnGraphicScene *scene;
  OwnGraphicView *overlay;

  int getRateValue();
  std::shared_ptr<AnnotatorLib::Session> getSession() const;
  std::shared_ptr<AnnotatorLib::Project> getProject() const;

 public slots:
  void showFrame(cv::Mat frame);
  void updateFrame(long frame);
  void showAnnotationsOfFrame(shared_ptr<AnnotatorLib::Frame> frame);
  void setSliderValue(int newpos);
  // void on_objectSelected(shared_ptr<AnnotatorLib::Object> obj);
  void on_frameSelected(long index);  // Jump to a position
  void on_autoAnnotate(bool);
  void reload();
  void enableDrawing(bool enable);
  void on_nextFrame(long frame);

  bool eventFilter(QObject *object, QEvent *event);

 signals:
  void signal_objectSelection(shared_ptr<AnnotatorLib::Object> object);

 protected:
  void clearAnnotationsGraphics();
  void runPlugin(unsigned long f);

 protected slots:
  void setInputCoordinate(QPoint point);

 private slots:
  void on_horizontalSlider_sliderMoved(int pos);
  void on_speedSpinBox_valueChanged(int arg1);  // change video playback speed
  void on_updateBtn();
  void on_btnPlay_clicked();
  void sleep(int msecs);  // sleep for a while

  void on_btnPause_clicked();

  void on_btnStop_clicked();

  void on_btnPrev_clicked();

  void on_btnNext_clicked();

  void updateHorizontalSlider();

 private:
  bool autoAnnotation = false;
  Ui::Player *ui;
  std::shared_ptr<AnnotatorLib::Project> project = nullptr;
  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
  QLabel *rateLabel;

  cv::Mat currentFrame;

  std::list<AnnotationGraphicsItem *> annotationGraphics;

  void updateStatus(bool var);
  void updateTimeLabel();
};

#endif  // PLAYER_H
