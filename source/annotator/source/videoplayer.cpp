#include "videoplayer.h"
#include <QDebug>

Videoplayer::Videoplayer(QObject *parent)
    : QThread(parent), delay(-1), stop(true) {
  connect(this, SIGNAL(revert()), this, SLOT(revertVideo()));
}

/**
 * getFrameRate	-	return the frame rate pro second
 *
 *
 * @return the frame rate
 */
double Videoplayer::getFrameRate() { return this->imageSet->getFPS(); }

/**
 * getTotalFrameNr	-	return the total frame number
 *
 *
 * @return the total frame number
 */
long Videoplayer::getTotalFrameNr() { return imageSet->size(); }

/**
 * getCurFrameNr	-	return the current frame number
 *
 *
 * @return the current frame number
 */
long Videoplayer::getCurFrameNr() { return imageSet->getPosition(); }

/**
 * getPositionMS	-	return the position in milliseconds
 *
 * @return the position in milliseconds
 */
double Videoplayer::getPositionMS() {
  return 1000.0 * imageSet->getPosition() / this->getFrameRate();
}

/**
 * getLengthMS	-	return the video length in milliseconds
 *
 *
 * @return the length of video in milliseconds
 */
double Videoplayer::getLengthMS() {
  return 1000.0 * imageSet->size() / this->getFrameRate();
}

/**
 * isStop	-	Is the processing stop
 *
 *
 * @return True if not processing/playing. False otherwise
 */
bool Videoplayer::isStop() { return stop; }

void Videoplayer::setStop(bool state) { stop = state; }

/**
 * isOpened	-	Is the player opened
 *
 *
 * @return True if opened. False otherwise
 */
bool Videoplayer::isOpened() { return imageSet != nullptr; }

/**
 * getNextFrame	-	get the next frame if any
 *
 * @param frame	-	the expected frame
 *
 * @return True if success. False otherwise
 */
bool Videoplayer::getNextFrame(cv::Mat &frame) {
  if (!imageSet->hasNext()) return false;
  frame = imageSet->next();
  return true;
}

bool Videoplayer::getFrame(cv::Mat &frame) {
  try {
    frame = imageSet->getImage(imageSet->getPosition());
  } catch (...) {
    return false;
  }
  return true;
}

/**
 * setDelay	-	 set a delay between each frame
 *
 * 0 means wait at each frame,
 * negative means no delay
 * @param d	-	delay param
 */
void Videoplayer::setDelay(int d) { delay = d; }

void Videoplayer::run() { playIt(); }

/**
 * playIt	-	play the frames of the sequence
 *
 */
void Videoplayer::playIt() {
  // is playing
  setStop(false);
  emit updateBtn_signal();
  while (!isStop() && imageSet->hasNext()) {
    start_Timer.start();
    nextFrame();
    emit nextFrame(getCurFrameNr());
    sleep(delay);
  }
  // if video ended and not stopped jump to first frame
  if (!isStop()) {
    emit revert();
  }
}

/**
 * wait	-	pause the video for msecs
 *
 * @param msecs	-	msecs to should be wait
 */
void Videoplayer::wait(int msecs) {
  double stop = start_Timer.elapsed();
  int wait = std::max(msecs - stop, 1.0);

  QTime dieTime = QTime::currentTime().addMSecs(wait);
  while (QTime::currentTime() < dieTime) {
    QThread::msleep(1);
    QApplication::processEvents(QEventLoop::AllEvents, 1);
  }
}

/**
 * on_frameSelected	-	Jump to a position
 *
 * @param index	-	frame index
 *
 * @return True if success. False otherwise
 */
bool Videoplayer::on_frameSelected(long index) {
  if (index > getTotalFrameNr()) {
    return 1;
  } else if (index == getTotalFrameNr()) {
    index--;
  }

  bool re = imageSet->gotoPosition(index);

  if (re) {
    cv::Mat frame;
    getFrame(frame);
    emit showFrame(frame);
    emit updateFrame(getCurFrameNr());
  }
  start_Timer.restart();
  return re;
}

void Videoplayer::reload() { on_frameSelected(getCurFrameNr()); }

/**
 * pauseIt	-	pause playing
 *
 */
void Videoplayer::pauseIt() {
  stop = true;
  emit updateBtn_signal();
}

/**
 * stopIt	-	stop playing or processing
 *
 */
void Videoplayer::stopIt() {
  stop = true;
  emit revert();
}

/**
 * revertVideo	-	revert playing
 *
 */
void Videoplayer::revertVideo() { on_frameSelected(0); }

/**
 * close	-	close the video
 *
 */
void Videoplayer::close() { imageSet = nullptr; }

/**
 * nextFrame	-	display the next frame of the sequence
 *
 */
void Videoplayer::nextFrame() {
  if (getCurFrameNr() < getTotalFrameNr()) {
    on_frameSelected(getCurFrameNr() + 1);
  }
}

/**
 * prevFrame	-	display the prev frame of the sequence
 *
 */
void Videoplayer::prevFrame() {
  if (getCurFrameNr() > 0) {
    on_frameSelected(getCurFrameNr() - 1);
  }
}

void Videoplayer::setImageSet(
    std::shared_ptr<AnnotatorLib::ImageSet::AbstractImageSet> imageSet) {
  this->imageSet = imageSet;
  if (this->imageSet) {
    reload();
  }
}
