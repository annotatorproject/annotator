#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

#include <AnnotatorLib/Project.h>
#include <AnnotatorLib/Session.h>
#include <QLabel>
#include <list>

#include <memory>

// Graphics
#include "graphic/owngraphicscene.h"
#include "graphic/owngraphicview.h"

// OPENCV//
#include <videoplayer.h>
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
  void setProject(AnnotatorLib::Project *project);
  bool LoadFile(const QString &fileName);
  void play();
  void pause();
  Videoplayer *videoplayer;  // videoplayer instance

  OwnGraphicScene *scene;
  OwnGraphicView *overlay;

  QString getRateValue();

  void selectObject(shared_ptr<AnnotatorLib::Object> object);

  AnnotatorLib::Session *getSession();

  void setAutoAnnotation(bool autoAnnotation);

 public slots:
  void showFrame(cv::Mat frame);
  void updateFrame(long frame);
  void showAnnotationsOfFrame(shared_ptr<AnnotatorLib::Frame> frame);
  void setSliderValue(int newpos);
  void jumpTo(long index);  // Jump to a position
  void reload();

 signals:
  void objectSelected(shared_ptr<AnnotatorLib::Object> object);
  void requestReload();

 protected:
  void loadVideo(QString fileName);
  void clearAnnotationsGraphics();

 protected slots:
  void setInputCoordinate(QPoint point);

private slots:
  void on_horizontalSlider_sliderMoved(int pos);
  void on_speedSpinBox_valueChanged(int arg1);   //change video playback speed
  void updateBtn();
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
  AnnotatorLib::Project *project = nullptr;
  AnnotatorLib::Session *session = nullptr;
  QLabel *rateLabel;

  cv::Mat currentFrame;

  std::list<shared_ptr<AnnotationGraphicsItem>> annotationGraphics;

  void updateStatus(bool var);
  void updateTimeLabel();
};

#endif  // PLAYER_H
