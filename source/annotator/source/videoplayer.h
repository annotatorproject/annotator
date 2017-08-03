#ifndef VIDEOPLAYER
#define VIDEOPLAYER

#include <QDateTime>
#include <QObject>
#include <QPixmap>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <QApplication>
#include <QCoreApplication>
#include <QEventLoop>
#include <QTime>
#include <QtCore/QThread>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <AnnotatorLib/Frame.h>
#include <AnnotatorLib/ImageSet/AbstractImageSet.h>

class Videoplayer : public QThread {
  Q_OBJECT

 public:
  explicit Videoplayer(QObject *parent = 0);

  double getFrameRate();   // return the frame rate
  long getTotalFrameNr();  // return the total frame number
  long getCurFrameNr();    // return the current frame number
  double getPositionMS();  // return the position in milliseconds
  double getLengthMS();    // return the video length in milliseconds
  bool isStop();           // return is the player playing
  bool isOpened();         // return is the player opened
  void playIt();           // play the frames of the sequence
  void setDelay(int d);    // set a delay between each frame in [ms]
  // void setFrameRate(int r);// set a specific framerate
  void pauseIt();    // pause the frames of the sequence
  void stopIt();     // Stop playing
  void close();      // close video
  void nextFrame();  // display the next frame of the sequence
  void prevFrame();  // display the prev frame of the sequence

  void setImageSet(
      std::shared_ptr<AnnotatorLib::ImageSet::AbstractImageSet> imageSet);

  void wait(int msecs);

  bool isImages;  // if load file is sequence of images
  void setStop(bool state);

  QTime start_Timer;
  // double rate;  // video frame rate

 public slots:
  bool on_frameSelected(long index);  // Jump to a position
  void reload();

 protected:
  void run();

 private slots:
  void revertVideo();

 signals:

  void nextFrame(long frame);
  void updateBtn();               // update button status
  void updateBtn_signal();        // update button status
  void showFrame(cv::Mat frame);  // show a frame
  void updateFrame(long frame);
  void updateHorizontalSlider();  // update horizontal Slider
  void sleep(int msecs);          // sleep for a while
  void revert();
  void imageon_frameSelected(long pos);  // jump to position for images
  void showImage(QPixmap img);
  void setInputCoordinate(
      QPoint point);  // copy the image/video coordinate to main

 private:
  int delay;  // delay between each frame processing

  bool stop;  // to stop the player

  int curIndex;  // current index for output images

  bool getNextFrame(cv::Mat &frame);  // get the next frame if any

  /**
   * @brief getFrame
   * @param frame update to current frame
   * @return true if frame could read, false if exception occured
   */
  bool getFrame(cv::Mat &frame);

  std::shared_ptr<AnnotatorLib::ImageSet::AbstractImageSet> imageSet = nullptr;
};

#endif  // VIDEOPLAYER
