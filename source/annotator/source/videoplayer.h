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
#include <AnnotatorLib/ImageSet.h>

class Videoplayer : public QThread {
  Q_OBJECT

 public:
  explicit Videoplayer(QObject *parent = 0);

  bool setInput(const std::string &fileName);  // set the name of the video file
  double getFrameRate();                       // return the frame rate
  long getTotalFrameNr();                      // return the total frame number
  long getCurFrameNr();    // return the current frame number
  double getPositionMS();  // return the position in milliseconds
  double getLengthMS();    // return the video length in milliseconds
  bool isStop();           // return is the player playing
  bool isOpened();         // return is the player opened
  void playIt();           // play the frames of the sequence
  void setDelay(int d);    // set a delay between each frame
  void pauseIt();          // pause the frames of the sequence
  void stopIt();           // Stop playing
  void close();            // close video
  void nextFrame();        // display the next frame of the sequence
  void prevFrame();        // display the prev frame of the sequence

  void setImageSet(AnnotatorLib::ImageSet *imageSet);

  void wait(int msecs);

  void setLength(long size);

  bool isImages;  // if load file is sequence of images
  void setCurImageNr(long Nr);
  void setStop(bool state);

  QTime start_Timer;
  double rate;  // video frame rate

 public slots:
  bool jumpTo(long index);  // Jump to a position
  void reload();

 protected:
  void run();

 private slots:
  void revertVideo();

 signals:

  void updateBtn();               // update button status
  void showFrame(cv::Mat frame);  // show a frame
  void showFrame(long frame);
  void updateHorizontalSlider();  // update horizontal Slider
  void sleep(int msecs);          // sleep for a while
  void revert();
  void imageJumpTo(long pos);  // jump to position for images
  void showImage(QPixmap img);
  void setInputCoordinate(
      QPoint point);  // copy the image/video coordinate to main

 private:
  int delay;  // delay between each frame processing

  long fnumber;  // number of processed frames

  long length;  // total number of frames

  bool stop;  // to stop the player

  bool modify;  // is the video modified

  long curPos;  // the current playing pos

  int curIndex;  // current index for output images

  int curLevel;  // current level of pyramid

  int digits;  // number of digits in output image filename

  std::string extension;  // extension of output images

  int levels;  // level numbers of image pyramid

  float alpha;  // amplification factor

  float lambda_c;  // cut-off wave length

  float fl;  // low cut-off

  float fh;  // high cut-off

  float chromAttenuation;  // chromAttenuation

  float delta;  // delta

  float exaggeration_factor;  // extraggon factor

  float lambda;  // lambda

  std::string tempFile;  // temp filename

  bool getNextFrame(cv::Mat &frame);  // get the next frame if any

  AnnotatorLib::ImageSet *imageSet = nullptr;
};

#endif  // VIDEOPLAYER
