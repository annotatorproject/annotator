#include "player.h"
#include "ui_player.h"

#include <AnnotatorLib/Algo/InterpolateAnnotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "geomObject/annotationgraphicsitemfactory.h"
#include "plugins/pluginloader.h"
#include "controller/commandcontroller.h"

Player::Player(QWidget *parent) : QWidget(parent), ui(new Ui::Player) {
  ui->setupUi(this);
  video = new Videoplayer;  // Creating an instance of class videoplayer

  ui->horizontalSlider->setMaximum(9999);
  ui->btnNext->setAutoRepeat(true);
  ui->btnPrev->setAutoRepeat(true);

  video->setDelay(ui->speedSpinBox->value());  //get default value

  updateStatus(false);  // set Status disable
  updateBtn();          // update the button Play & Pause

  connect(video, SIGNAL(showFrame(cv::Mat)), this, SLOT(showFrame(cv::Mat)));
  connect(video, SIGNAL(showFrame(long)), this, SLOT(showFrame(long)));
  connect(ui->horizontalSlider, SIGNAL(sendClickPosition(int)), this,
          SLOT(setSliderValue(int)));
  connect(video, SIGNAL(updateBtn()), this, SLOT(updateBtn()));
  connect(video, SIGNAL(sleep(int)), this, SLOT(sleep(int)));
  connect(video, SIGNAL(updateHorizontalSlider()), this,
          SLOT(updateHorizontalSlider()));
  connect(video, SIGNAL(setInputCoordinate(QPoint)), this,
          SLOT(setInputCoordinate(QPoint)));

  // command controller
  connect(CommandController::instance(), SIGNAL(onExecute()), this, SLOT(reload()));
  connect(CommandController::instance(), SIGNAL(onUndo()), this, SLOT(reload()));

  // create graphicsview to be able to draw objects on screen.
  scene = new OwnGraphicScene();
  overlay = new OwnGraphicView(scene, this);
  // overlay->setEnabled(false);
  ui->playerLayout->addWidget(overlay);
  overlay->show();

  connect(scene, SIGNAL(on_btnPause_clicked()), this,
          SLOT(on_btnPause_clicked()));

}

Player::~Player() {
  clearAnnotationsGraphics();
  delete ui;
}

/**
 * setInputCoordinate	-	set image/video coordinate into scene to adjust
 * the coordinate system.
 */
void Player::setInputCoordinate(QPoint point) {
  scene->inputCoordinate = point;
}

void Player::setRateLabel(QLabel *label) { this->rateLabel = label; }

QString Player::getRateValue() {
  return "FPS: " + QString::number(video->rate);
}

void Player::selectObject(AnnotatorLib::Object *object) {
  if (object != nullptr) emit objectSelected(object);
}

AnnotatorLib::Session *Player::getSession() { return this->session; }

void Player::setAutoAnnotation(bool autoAnnotation) {
  this->autoAnnotation = autoAnnotation;
}

void Player::setProject(AnnotatorLib::Project *project) {
  this->project = project;
  this->session = project->getSession();

  video->setImageSet(project->getImageSet());
  cv::Mat firstImage = project->getImageSet()->next();
  project->getImageSet()->gotoPosition(0);
  scene->setSceneRect(0, 0, firstImage.cols, firstImage.rows);
  scene->setSession(session);
  overlay->fitInView(scene->sceneRect());

  updateStatus(true);
  updateBtn();

  overlay->setEnabled(true);

  updateTimeLabel();
}

void Player::loadVideo(QString fileName) {
  if (!fileName.isEmpty()) {
    if (LoadFile(fileName)) {
      updateStatus(true);
      updateBtn();
      overlay->setEnabled(true);
    }
  }
}

void Player::clearAnnotationsGraphics() {
  while(!annotationGraphics.empty()) {
    delete annotationGraphics.front();
    annotationGraphics.pop_front();
  }
  scene->update();
}

/**
 * LoadFile	-	Open and load a video
 *
 * @param fileName	-	file Path
 *
 * @return true if the file is successfully loaded
 */
bool Player::LoadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, tr("VideoPlayer"),
                         tr("Unable to load file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    return false;
  }

  // change the cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);

  // input file
  if (!video->setInput(fileName.toStdString())) {
    QMessageBox::warning(this, tr("VideoPlayer"),
                         tr("Unable to load file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
    return false;
  }

  // restore the cursor
  QApplication::restoreOverrideCursor();

  // update the time label
  updateTimeLabel();

  return true;
}

/**
 * updateStatus	-	Update the enabled property of menu
 *
 * @param var	-	the enable variable
 */
void Player::updateStatus(bool enable) {
  ui->horizontalSlider->setEnabled(enable);
  ui->btnPrev->setEnabled(enable);
  ui->btnNext->setEnabled(enable);
  ui->btnPlay->setEnabled(enable);
  ui->btnStop->setEnabled(enable);
  ui->btnPause->setEnabled(enable);

  if (!enable) {
    ui->horizontalSlider->setValue(0);
  }
}

/**
 * updateBtn	-	update the button Play & Pause
 *
 */
void Player::updateBtn() {
  bool isStop = video->isStop();
  ui->btnPause->setVisible(!isStop);
  ui->btnPlay->setVisible(isStop);
}

/**
 * showFrame	-	show a frame
 *
 * @param image	-	the image to be showed
 *
 */
void Player::showFrame(cv::Mat frame) {
  currentFrame = frame;
  cv::Mat tmp;

  cv::Size size;
  size.width = scene->sceneRect().width();
  size.height = scene->sceneRect().height();

  tmp.convertTo(tmp, CV_8U);
  cv::resize(currentFrame, tmp, size);
  cvtColor(currentFrame, tmp, CV_BGR2RGB);

  QImage img = QImage((const unsigned char *)(tmp.data), tmp.cols, tmp.rows,
                      tmp.step, QImage::Format_RGB888);

  scene->setSceneRect(img.rect());
  overlay->fitInView(img.rect());
  QPixmap pim = QPixmap::fromImage(img);
  scene->setBackgroundBrush(pim);
}

void Player::showFrame(long frame) {
  AnnotatorLib::Frame *f = session->getFrame(frame);
  if (f == nullptr) f = new AnnotatorLib::Frame(frame);

  if (autoAnnotation) {
    if (f != nullptr) {
      Annotator::Plugin *plugin =
          Annotator::PluginLoader::getInstance().getCurrent();
      if (plugin) {
        plugin->setFrame(f, currentFrame);
      }
      algoExecuteCommands();
      showTrackedAnnotations(f); //only active when plugin was loaded //rename this method??
    }
  }

  showAnnotationsOfFrame(f);

  // set current frame into popup.
  //long cfn = video->getCurFrameNr();
  this->scene->setCurrentFrame(frame);
}

void Player::showAnnotationsOfFrame(AnnotatorLib::Frame *frame) {
  clearAnnotationsGraphics();
  if (!autoAnnotation && frame != nullptr) {
    for (AnnotatorLib::Annotation *annotation :
         AnnotatorLib::Algo::InterpolateAnnotation::getInterpolations(
             frame, this->session)) {
      AnnotationGraphicsItem *graphicsItem =
          AnnotationGraphicsItemFactory::createItem(annotation);

      /*scene->setItem(QPointF(graphicsItem->annotation->getX()-graphicsItem->annotation->getHRadius(),
         graphicsItem->annotation->getX()-graphicsItem->annotation->getHRadius()),
                     QPointF(graphicsItem->annotation->getX()+graphicsItem->annotation->getHRadius(),
         graphicsItem->annotation->getX()+graphicsItem->annotation->getHRadius()),
                     2333, QColor(255,0,0,255));*/
      graphicsItem->setPlayer(this);
      scene->addItem(graphicsItem);
      scene->update();
      annotationGraphics.push_back(graphicsItem);
    }
  }
}

void Player::algoExecuteCommands() {
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();

  if (plugin) {
    std::vector<AnnotatorLib::Commands::Command *> commands =
        plugin->getCommands();
    for (AnnotatorLib::Commands::Command *command : commands) {
      CommandController::instance()->execute(command);
    }
  }
}

void Player::showTrackedAnnotations(AnnotatorLib::Frame *frame) {
  if (frame != nullptr) {
    Annotator::Plugin *plugin =
        Annotator::PluginLoader::getInstance().getCurrent();
    AnnotatorLib::Annotation *a = nullptr;

    if (plugin) {
      a = plugin->getAnnotation();
    }
    if (a) {
      int x = a->getX();
      int y = a->getY();
      int w = a->getHRadius();
      int h = a->getVRadius();

      AnnotatorLib::Commands::NewAnnotation *nA =
          new AnnotatorLib::Commands::NewAnnotation(
              a->getObject(), frame, x, y, w, h, a->getNext(),
              a->isInterpolated() ? a->getPrevious() : a, getSession(), false);
      CommandController::instance()->execute(nA);

      // update position of last annotation in automation plugin
      if (plugin->getObject() == a->getObject()) {
        plugin->setLastAnnotation(nA->getAnnotation());
      }
    }
  }
}

/**
 * updateHorizontalSlider	-	update the progress bar
 *
 */
void Player::updateHorizontalSlider() {
  // update the progress bar
  ui->horizontalSlider->setValue((double)video->getCurFrameNr() *
                                 ui->horizontalSlider->maximum() /
                                 video->getTotalFrameNr() * 1.0);
  // update the time label
  updateTimeLabel();
}

void Player::on_horizontalSlider_sliderMoved(int newpos)
{
   long pos = newpos * video->getTotalFrameNr() / ui->horizontalSlider->maximum();

   video->jumpTo(pos);
   updateTimeLabel();
}

/**
 * setSliderValue	-	set new value on the progress bar
 *
*/
void Player::setSliderValue(int newpos) {
    ui->horizontalSlider->setValue(newpos);
    on_horizontalSlider_sliderMoved(newpos);
}

void Player::jumpTo(long index) { this->video->jumpTo(index); }

/**
 * updateTimeLabel	-	update Time Label
 */
void Player::updateTimeLabel() {
  QString curPos =
      QDateTime::fromMSecsSinceEpoch(video->getPositionMS() - 3600000)
          .toString("hh:mm:ss");

  QString length =
      QDateTime::fromMSecsSinceEpoch(video->getLengthMS() - 3600000)
          .toString("hh:mm:ss");

  ui->timeLabel->setText(tr("<span style=' color:#FFCD00;'>"
                            "%1</span> / %2")
                             .arg(curPos, length));

  long cfn = video->getCurFrameNr();
  long tfn = video->getTotalFrameNr();

  ui->frameNrLabel->setText(
      tr("<span style=' color:#FFCD00;'>"
         "%1</span> / %2")
          .arg(QString::number(cfn), QString::number(tfn)));
}

/**
 * play	-	play the video
 *
 */
void Player::play() {
  //video->setDelay(1000.f / video->getFrameRate());
  video->playIt();
}

/**
 * pause	-	pause the video
 *
 */
void Player::pause() {
  video->pauseIt();
}

void Player::on_btnPlay_clicked() { play(); }

/**
 * sleep	-	pause the video for several msecs
 *
 * @param msecs
 */
void Player::sleep(int msecs) { video->wait(msecs); }

void Player::on_btnPause_clicked() { pause(); }

void Player::on_btnStop_clicked() { video->stopIt(); }

void Player::on_btnPrev_clicked() {
  //this->setAutoAnnotation(false);
  video->prevFrame();
}

void Player::on_btnNext_clicked() { video->nextFrame(); }

///#################################################################################################///

void Player::reload() {
  emit requestReload();
  this->video->reload();
}


/**
 * update the video speed by a factor f in percent
 *
 */
void Player::on_speedSpinBox_valueChanged(int f)
{
    //TODO: set proper framerate
    video->setDelay( (100.f * video->getFrameRate()) / (float) f);
}


