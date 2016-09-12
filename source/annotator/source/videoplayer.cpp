#include "videoplayer.h"
#include <QDebug>

Videoplayer::Videoplayer(QObject *parent)
    : QThread(parent),
      rate(0),
      delay(-1),
      fnumber(0),
      length(0),
      stop(true),
      modify(false),
      curPos(0),
      curIndex(0),
      curLevel(0),
      digits(0),
      extension(".avi"),
      levels(4),
      alpha(10),
      lambda_c(80),
      fl(0.05),
      fh(0.4),
      chromAttenuation(0.1),
      delta(0),
      exaggeration_factor(2.0),
      lambda(0) {
  connect(this, SIGNAL(revert()), this, SLOT(revertVideo()));
}

/**
 * setInput	-	set the name of the expected video file
 *
 * @param fileName	-	the name of the video file
 *
 * @return True if success. False otherwise
 */
bool Videoplayer::setInput(const std::string &fileName) {
  fnumber = 0;
  tempFile = fileName;

  // In case a resource was already
  // associated with the VideoCapture instance
  rate = getFrameRate();

  cv::Mat input;

  // show first frame
  getNextFrame(input);
  emit setInputCoordinate(QPoint(input.cols, input.rows));

  emit showFrame(input);
  emit updateFrame(getCurFrameNr());

  emit updateBtn();
  return true;
}

/**
 * getFrameRate	-	return the frame rate pro second
 *
 *
 * @return the frame rate
 */
double Videoplayer::getFrameRate() {
  // TODO:
  // return capture.get(CV_CAP_PROP_FPS);
  return 25;
}

/**
 * getTotalFrameNr	-	return the total frame number
 *
 *
 * @return the total frame number
 */
long Videoplayer::getTotalFrameNr() {
  // length = capture.get(CV_CAP_PROP_FRAME_COUNT);
  length = imageSet->size();
  return length;
}

void Videoplayer::setLength(long size) { length = size; }

/**
 * getCurFrameNr	-	return the current frame number
 *
 *
 * @return the current frame number
 */
long Videoplayer::getCurFrameNr() {
  // curPos = capture.get(CV_CAP_PROP_POS_FRAMES);
  curPos = imageSet->getPosition();
  return curPos;
}

void Videoplayer::setCurImageNr(long Nr) { curPos = Nr; }

/**
 * getPositionMS	-	return the position in milliseconds
 *
 * @return the position in milliseconds
 */
double Videoplayer::getPositionMS() {
  return imageSet->getPosition() * this->getFrameRate();
}

/**
 * getLengthMS	-	return the video length in milliseconds
 *
 *
 * @return the length of video in milliseconds
 */
double Videoplayer::getLengthMS() {
  double l = 1000.0 * length / rate;

  return l;
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
  // return capture.read(frame);
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
  // current frame
  cv::Mat input;

  // if no capture device has been set
  if (!isOpened()) return;

  // is playing
  setStop(false);

  // update buttons
  emit updateBtn();

  while (!isStop()) {
    // capture time needed to process 1 frame.
    start_Timer.start();

    // read next frame if any
    if (!getNextFrame(input)) break;

    curPos = imageSet->getPosition();

    // display input frame
    emit showFrame(input);
    emit updateFrame(getCurFrameNr());

    // update the progress bar
    emit updateHorizontalSlider();

    // introduce a delay
    emit sleep(delay);
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
  int wait = cv::max(msecs - stop, 1.0);

  QTime dieTime = QTime::currentTime().addMSecs(wait);
  while (QTime::currentTime() < dieTime)
    QApplication::processEvents(QEventLoop::AllEvents, 1);
}

/**
 * on_frameSelected	-	Jump to a position
 *
 * @param index	-	frame index
 *
 * @return True if success. False otherwise
 */
bool Videoplayer::on_frameSelected(long index) {
  if (index > length) {
    return 1;
  } else if (index == length) {
    index--;
  }

  bool re = imageSet->gotoPosition(index);

  if (re) {
    cv::Mat frame;
    getNextFrame(frame);
    emit showFrame(frame);
    curPos = index; //do we need this?
    emit updateFrame(getCurFrameNr());
  }
  start_Timer.restart();
  return re;
}

void Videoplayer::reload() { on_frameSelected(this->curPos - 1); }  // TODO: why -1?

/**
 * pauseIt	-	pause playing
 *
 */
void Videoplayer::pauseIt() {
  stop = true;
  emit updateBtn();
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
void Videoplayer::revertVideo() {
  on_frameSelected(0);
  emit updateHorizontalSlider();
}

/**
 * close	-	close the video
 *
 */
void Videoplayer::close() {
  rate = 0;
  length = 0;
  imageSet = nullptr;
}

/**
 * nextFrame	-	display the next frame of the sequence
 *
 */
void Videoplayer::nextFrame() {
  if (curPos < length) {
    on_frameSelected(curPos);
  }
  emit updateHorizontalSlider();
}

/**
 * prevFrame	-	display the prev frame of the sequence
 *
 */
void Videoplayer::prevFrame() {
  if (curPos >= 0) {
      curPos -= 2;
      on_frameSelected(curPos);
  }
  emit updateHorizontalSlider();
}

void Videoplayer::setImageSet(AnnotatorLib::ImageSet *imageSet) {
  this->imageSet = imageSet;
}
