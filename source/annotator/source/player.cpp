#include "player.h"
#include <AnnotatorLib/Algo/InterpolateAnnotation.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Frame.h>
#include <geomObject/annotationgraphicsitem.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "controller/commandcontroller.h"
#include "controller/selectioncontroller.h"
#include "geomObject/annotationgraphicsitemfactory.h"
#include "plugins/pluginloader.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QWidget(parent), ui(new Ui::Player) {
  ui->setupUi(this);
  videoplayer = new Videoplayer();  // Creating an instance of class videoplayer

  ui->btnNext->setAutoRepeat(true);
  ui->btnPrev->setAutoRepeat(true);

  videoplayer->setDelay(ui->speedSpinBox->value());  // get default value

  updateStatus(false);  // set Status disable
  on_updateBtn();       // update the button Play & Pause

  connect(videoplayer, SIGNAL(nextFrame(long)), this, SLOT(on_nextFrame(long)));
  connect(videoplayer, SIGNAL(showFrame(cv::Mat)), this,
          SLOT(showFrame(cv::Mat)));
  connect(videoplayer, SIGNAL(updateFrame(long)), this,
          SLOT(updateFrame(long)));
  connect(ui->horizontalSlider, SIGNAL(sendClickPosition(int)), this,
          SLOT(setSliderValue(int)));
  connect(videoplayer, SIGNAL(updateBtn_signal()), this, SLOT(on_updateBtn()));
  connect(videoplayer, SIGNAL(sleep(int)), this, SLOT(sleep(int)));
  connect(videoplayer, SIGNAL(updateHorizontalSlider()), this,
          SLOT(updateHorizontalSlider()));
  connect(videoplayer, SIGNAL(setInputCoordinate(QPoint)), this,
          SLOT(setInputCoordinate(QPoint)));
  // command controller
  connect(CommandController::instance(), SIGNAL(signal_requestFrameRedraw()),
          this, SLOT(reload()));

  // create graphicsview to be able to draw objects on screen.
  scene = new OwnGraphicScene();
  overlay = new OwnGraphicView(scene, this);
  // overlay->setEnabled(false);
  ui->playerLayout->addWidget(overlay);
  overlay->show();

  connect(scene, SIGNAL(on_btnPause_clicked()), this,
          SLOT(on_btnPause_clicked()));

  connect(
      this, SIGNAL(signal_objectSelection(shared_ptr<AnnotatorLib::Object>)),
      scene, SLOT(on_signal_objectSelection(shared_ptr<AnnotatorLib::Object>)));

  qApp->installEventFilter(this);
}

Player::~Player() {
  clearAnnotationsGraphics();
  delete videoplayer;
  delete scene;
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

int Player::getRateValue() { return (int)videoplayer->getFrameRate(); }

std::shared_ptr<AnnotatorLib::Session> Player::getSession() const {
  return this->session;
}

void Player::closeProject() {
  this->currentFrame = cv::Mat();
  setProject(nullptr);
  this->videoplayer->close();
  scene->setSession(nullptr);
  updateStatus(false);  // disable ui
}

std::shared_ptr<AnnotatorLib::Project> Player::getProject() const {
  return project;
}

void Player::setProject(std::shared_ptr<AnnotatorLib::Project> project) {
  this->project = project;

  this->session = project ? project->getSession() : nullptr;

  ui->horizontalSlider->setMaximum(project ? project->getImageSet()->size()
                                           : 0);

  videoplayer->setImageSet(project ? project->getImageSet() : nullptr);
  ui->horizontalSlider->setMaximum(project ? project->getImageSet()->size()
                                           : 0);

  cv::Mat firstImage = project ? project->getImageSet()->next() : cv::Mat();
  scene->setSceneRect(0, 0, firstImage.cols, firstImage.rows);
  scene->setSession(project ? session : nullptr);

  this->setEnabled(project.get() != nullptr);

  if (project) {
    overlay->fitInView(scene->sceneRect());
    project->getImageSet()->gotoPosition(0);
  }

  updateStatus(true);
  on_updateBtn();
  updateTimeLabel();
}

void Player::clearAnnotationsGraphics() {
  while (!annotationGraphics.empty()) {
    delete annotationGraphics.front();
    annotationGraphics.pop_front();
  }
  scene->update();
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

/**get()
 * updateBtn	-	update the button Play & Pause
 *
 */
void Player::on_updateBtn() {
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
  //  cv::Mat tmp;

  //  cv::Size size;
  //  size.width = scene->sceneRect().width();
  //  size.height = scene->sceneRect().height();

  //  tmp.convertTo(tmp, CV_8U);
  //  cv::resize(currentFrame, tmp, size);
  //  cvtColor(currentFrame, tmp, CV_BGR2RGB);
  //    QImage img = QImage((const unsigned char *)(tmp.data), tmp.cols,
  //    tmp.rows,
  //                        tmp.step, QImage::Format_RGB888);

  cvtColor(currentFrame, currentFrame, CV_BGR2RGB);
  QImage img = QImage((const unsigned char *)(frame.data), frame.cols,
                      frame.rows, frame.step, QImage::Format_RGB888);

  scene->setSceneRect(img.rect());
  overlay->fitInView(img.rect());
  QPixmap pim = QPixmap::fromImage(img);
  scene->setBackgroundBrush(pim);
}

void Player::updateFrame(long frame_nmb) {
  shared_ptr<AnnotatorLib::Frame> f = session->getFrame(frame_nmb);
  if (!f)
    f = std::make_shared<AnnotatorLib::Frame>(
        frame_nmb);  // create temporary frame
  this->scene->setCurrentFrame(frame_nmb);
  showAnnotationsOfFrame(f);
  this->scene->update();
}

void Player::runPlugin(unsigned long frame_nmb) {
  shared_ptr<AnnotatorLib::Frame> f = session->getFrame(frame_nmb);
  if (!f)
    f = std::make_shared<AnnotatorLib::Frame>(
        frame_nmb);  // create temporary frame

  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();

  if (plugin) {
    for (shared_ptr<AnnotatorLib::Commands::Command> command :
         plugin->calculate(SelectionController::instance()->getSelectedObject(),
                           f, false)) {
      CommandController::instance()->execute(command);
    }
  }
}

void Player::showAnnotationsOfFrame(shared_ptr<AnnotatorLib::Frame> frame) {
  clearAnnotationsGraphics();

  for (shared_ptr<AnnotatorLib::Annotation> annotation :
       AnnotatorLib::Algo::InterpolateAnnotation::getInterpolations(
           this->session, frame)) {
    AnnotationGraphicsItem *graphicsItem =
        AnnotationGraphicsItemFactory::createItem(annotation);

    connect(SelectionController::instance(),
            SIGNAL(signal_objectSelection(shared_ptr<AnnotatorLib::Object>)),
            graphicsItem,
            SLOT(on_objectSelected(shared_ptr<AnnotatorLib::Object>)));

    graphicsItem->setPlayer(this);
    scene->addItem(graphicsItem);
    annotationGraphics.push_back(graphicsItem);
  }
}

/**
 * updateHorizontalSlider	-	update the progress bar
 *
 */
void Player::updateHorizontalSlider() {
  // update the progress bar
  ui->horizontalSlider->setValue(videoplayer->getCurFrameNr());
  updateTimeLabel();
}

void Player::on_horizontalSlider_sliderMoved(int newpos) {
  long pos =
      newpos * videoplayer->getTotalFrameNr() / ui->horizontalSlider->maximum();
  on_frameSelected(pos);
}

/**
 * setSliderValue	-	set new value on the progress bar
 *
*/
void Player::setSliderValue(int newpos) {
  ui->horizontalSlider->setValue(newpos);
  on_horizontalSlider_sliderMoved(newpos);
}

void Player::on_frameSelected(long index) {
  this->videoplayer->on_frameSelected(index);  // why -1?
  updateTimeLabel();
  updateHorizontalSlider();
}

/**
 * updateTimeLabel	-	update Time Label
 */
void Player::updateTimeLabel() {
  double pos_ms = 0;
  double length_ms = 0;
  long cfn = 0;
  long tfn = 0;
  if (this->videoplayer->isOpened()) {
    pos_ms = videoplayer->getPositionMS();
    length_ms = videoplayer->getLengthMS();
    cfn = videoplayer->getCurFrameNr();
    tfn = videoplayer->getTotalFrameNr();
  }
  QString curPos =
      QDateTime::fromMSecsSinceEpoch(pos_ms - 3600000).toString("hh:mm:ss");

  QString length =
      QDateTime::fromMSecsSinceEpoch(length_ms - 3600000).toString("hh:mm:ss");

  ui->timeLabel->setText(tr("<span style=' color:#FFCD00;'>"
                            "%1</span> / %2")
                             .arg(curPos, length));

  ui->frameNrLabel->setText(
      tr("<span style=' color:#FFCD00;'>"
         "%1</span> / %2")
          .arg(QString::number(cfn + 1), QString::number(tfn)));
}

/**
 * play	-	play the video
 *
 */
void Player::play() {
  // video->setDelay(1000.f / video->getFrameRate());
  videoplayer->playIt();
}

/**
 * pause	-	pause the video
 *
 */
void Player::pause() { videoplayer->pauseIt(); }

/**
 * sleep	-	pause the video for several msecs
 *
 * @param msecs
 */
void Player::sleep(int msecs) { videoplayer->wait(msecs); }

///#################################################################################################///

void Player::reload() {
  // this->videoplayer->reload();
  // reload annotations?
  this->updateFrame(this->videoplayer->getCurFrameNr());
}

void Player::enableDrawing(bool enable) {
  this->ui->playerScrollArea->setEnabled(enable);
}

void Player::on_nextFrame(long frame)  // wrong frame number???
{
  if (autoAnnotation) runPlugin(frame);
}

bool Player::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    if (object == scene || object == overlay ||
        object->objectName() == "playerScrollArea") {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      switch (keyEvent->key()) {
        case Qt::Key_Left:
          on_btnPrev_clicked();
          break;
        case Qt::Key_Right:
          on_btnNext_clicked();
          break;
        case Qt::Key_Space:
          break;
        default:
          break;
      };
    }
  }
  return QObject::eventFilter(object, event);
}

/**
 * update the video speed by a factor f in percent
 *
 */
void Player::on_speedSpinBox_valueChanged(int f) {
  videoplayer->setDelay((100.f * videoplayer->getFrameRate()) / (float)f);
}

///#########################################SLOTS###########################################///

void Player::on_btnPlay_clicked() { play(); }

void Player::on_btnPause_clicked() { pause(); }

void Player::on_btnStop_clicked() { videoplayer->stopIt(); }

void Player::on_btnPrev_clicked() {
  // this->setAutoAnnotation(false);
  videoplayer->prevFrame();
}

void Player::on_btnNext_clicked() {
  on_nextFrame(videoplayer->getCurFrameNr() + 1);
  videoplayer->nextFrame();
}

void Player::on_autoAnnotate(bool enabled) {
  if (enabled)
    this->ui->speedSpinBox->setValue(100);
  else
    this->ui->speedSpinBox->setValue(50);
  this->autoAnnotation = enabled;
  // don't allow to jump back and forth, when plugin is busy...
  this->ui->trackbarWidget->setEnabled(!enabled);
  this->ui->btnPrev->setEnabled(!enabled);
}
