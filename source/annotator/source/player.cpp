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
#include <geomObject/annotationgraphicsitem.h>
#include "plugins/pluginloader.h"
#include "controller/commandcontroller.h"

Player::Player(QWidget *parent) : QWidget(parent), ui(new Ui::Player) {
  ui->setupUi(this);
  videoplayer = new Videoplayer;  // Creating an instance of class videoplayer

  ui->horizontalSlider->setMaximum(9999);
  ui->btnNext->setAutoRepeat(true);
  ui->btnPrev->setAutoRepeat(true);

  videoplayer->setDelay(ui->speedSpinBox->value());  //get default value

  updateStatus(false);  // set Status disable
  updateBtn();          // update the button Play & Pause

  connect(videoplayer,
          SIGNAL(showFrame(cv::Mat)),
          this,
          SLOT(showFrame(cv::Mat)));
  connect(videoplayer,
          SIGNAL(updateFrame(long)),
          this,
          SLOT(updateFrame(long)));
  connect(ui->horizontalSlider,
          SIGNAL(sendClickPosition(int)),
          this,
          SLOT(setSliderValue(int)));
  connect(videoplayer,
          SIGNAL(updateBtn()),
          this,
          SLOT(updateBtn()));
  connect(videoplayer,
          SIGNAL(sleep(int)),
          this,
          SLOT(sleep(int)));
  connect(videoplayer,
          SIGNAL(updateHorizontalSlider()),
          this,
          SLOT(updateHorizontalSlider()));
  connect(videoplayer,
          SIGNAL(setInputCoordinate(QPoint)),
          this,
          SLOT(setInputCoordinate(QPoint)));

  // command controller
  connect(CommandController::instance(),
          SIGNAL(onCommandExecute()),
          this,
          SLOT(reload()));
  connect(CommandController::instance(),
          SIGNAL(onCommandUndo()),
          this,
          SLOT(reload()));

  // create graphicsview to be able to draw objects on screen.
  scene = new OwnGraphicScene();
  overlay = new OwnGraphicView(scene, this);
  // overlay->setEnabled(false);
  ui->playerLayout->addWidget(overlay);
  overlay->show();

  connect(scene, SIGNAL(on_btnPause_clicked()), this,
          SLOT(on_btnPause_clicked()));

  connect(this, SIGNAL(objectSelected(AnnotatorLib::Object*)), scene,
          SLOT(on_objectSelected(AnnotatorLib::Object*)));

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
  return "FPS: " + QString::number(videoplayer->rate);
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

  videoplayer->setImageSet(project->getImageSet());
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
    delete annotationGraphics.front();  //delete graphic and annotation if interpolated
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
  if (!videoplayer->setInput(fileName.toStdString())) {
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
  bool isStop = videoplayer->isStop();
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

void Player::updateFrame(long frame_nmb) {
  AnnotatorLib::Frame *f = session->getFrame(frame_nmb);

  showAnnotationsOfFrame(f);

  if (autoAnnotation) {
    Annotator::Plugin *plugin =
        Annotator::PluginLoader::getInstance().getCurrent();

    //TODO:
    if (plugin &&
        AnnotationGraphicsItem::getSelectedAnnotation() &&
        AnnotationGraphicsItem::getSelectedAnnotation()->getFrame() == f) {

        plugin->setLastAnnotation(AnnotationGraphicsItem::getSelectedAnnotation());
        plugin->setFrame(f, currentFrame);
        plugin->setObject(AnnotationGraphicsItem::getSelectedAnnotation()->getObject());
        //get commands and execute
        std::vector<AnnotatorLib::Commands::Command*> cmds = plugin->getCommands();
        for (auto it = cmds.begin(); it != cmds.end(); it++) {
          CommandController::instance()->execute(*it);
        }
    }
  }

  this->scene->setCurrentFrame(frame_nmb);
  this->scene->update();
}

void Player::showAnnotationsOfFrame(AnnotatorLib::Frame *frame) {
  clearAnnotationsGraphics();
  if (frame != nullptr) {
    for (AnnotatorLib::Annotation *annotation :
         AnnotatorLib::Algo::InterpolateAnnotation::getInterpolations(
             frame, this->session)) {
      AnnotationGraphicsItem *graphicsItem =
          AnnotationGraphicsItemFactory::createItem(annotation);

      graphicsItem->setPlayer(this);
      scene->addItem(graphicsItem);      
      annotationGraphics.push_back(graphicsItem);
    }
  }
}

/**
 * updateHorizontalSlider	-	update the progress bar
 *
 */
void Player::updateHorizontalSlider() {
  // update the progress bar
  ui->horizontalSlider->setValue((double)videoplayer->getCurFrameNr() *
                                 ui->horizontalSlider->maximum() /
                                 videoplayer->getTotalFrameNr() * 1.0);
  updateTimeLabel();
}

void Player::on_horizontalSlider_sliderMoved(int newpos)
{
   long pos = newpos * videoplayer->getTotalFrameNr() / ui->horizontalSlider->maximum();
   jumpTo(pos);

}

/**
 * setSliderValue	-	set new value on the progress bar
 *
*/
void Player::setSliderValue(int newpos) {
    ui->horizontalSlider->setValue(newpos);
    on_horizontalSlider_sliderMoved(newpos);
}

void Player::jumpTo(long index) {
  this->videoplayer->jumpTo(index - 1); //why -1?
  updateTimeLabel();
  updateHorizontalSlider();
}

/**
 * updateTimeLabel	-	update Time Label
 */
void Player::updateTimeLabel() {
  QString curPos =
      QDateTime::fromMSecsSinceEpoch(videoplayer->getPositionMS() - 3600000)
          .toString("hh:mm:ss");

  QString length =
      QDateTime::fromMSecsSinceEpoch(videoplayer->getLengthMS() - 3600000)
          .toString("hh:mm:ss");

  ui->timeLabel->setText(tr("<span style=' color:#FFCD00;'>"
                            "%1</span> / %2")
                             .arg(curPos, length));

  long cfn = videoplayer->getCurFrameNr();
  long tfn = videoplayer->getTotalFrameNr();

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
  videoplayer->playIt();
}

/**
 * pause	-	pause the video
 *
 */
void Player::pause() {
  videoplayer->pauseIt();
}

void Player::on_btnPlay_clicked() { play(); }

/**
 * sleep	-	pause the video for several msecs
 *
 * @param msecs
 */
void Player::sleep(int msecs) { videoplayer->wait(msecs); }

void Player::on_btnPause_clicked() { pause(); }

void Player::on_btnStop_clicked() { videoplayer->stopIt(); }

void Player::on_btnPrev_clicked() {
  //this->setAutoAnnotation(false);
  videoplayer->prevFrame();
}

void Player::on_btnNext_clicked() {
  videoplayer->nextFrame();
}

///#################################################################################################///

void Player::reload() {
  emit requestReload();
  this->videoplayer->reload();
  //TODO: reload annotations?
}


/**
 * update the video speed by a factor f in percent
 *
 */
void Player::on_speedSpinBox_valueChanged(int f)
{
    //TODO: set proper framerate
    videoplayer->setDelay( (100.f * videoplayer->getFrameRate()) / (float) f);
}


