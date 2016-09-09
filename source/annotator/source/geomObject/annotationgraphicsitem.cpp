#include "annotationgraphicsitem.h"
#include "controller/commandcontroller.h"
#include "gui/editobjectdialog.h"
#include "plugins/pluginloader.h"
#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/RemoveAnnotation.h>
#include <AnnotatorLib/Commands/RemoveObject.h>
#include <AnnotatorLib/Commands/CompressObject.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <gui/alert.h>
#include <QMenu>
#include <QObject>

// static
shared_ptr<AnnotatorLib::Annotation> AnnotationGraphicsItem::selected_annotation =
    shared_ptr<AnnotatorLib::Annotation>();
AnnotationGraphicsItem* AnnotationGraphicsItem::selected_annotation_item = nullptr;

void AnnotationGraphicsItem::setSelectedAnnotation(shared_ptr<AnnotatorLib::Annotation> a) {
  AnnotationGraphicsItem::selected_annotation.reset();
  AnnotationGraphicsItem::selected_annotation = a;
}

shared_ptr<AnnotatorLib::Annotation> AnnotationGraphicsItem::getSelectedAnnotation() {
  return AnnotationGraphicsItem::selected_annotation;
}

//constructor
AnnotationGraphicsItem::AnnotationGraphicsItem(
    shared_ptr<AnnotatorLib::Annotation> annotation)
    : QGraphicsItem::QGraphicsItem(nullptr), annotation(annotation) {
  assert(annotation);
  assert(annotation->getObject());

  originColor = idToColor(annotation->getObject()->getId());
  borderColor = originColor;

  if (isAnnotationSelected()) {
    borderColor = Qt::white;
  }

  rectX = 0;
  rectY = 0;
  width = annotation->getWidth();
  height = annotation->getHeight();
  setPos(annotation->getX(), annotation->getY());
  initCorners();
  initIdText();

  action_del = new QAction(QString("Remove"), (QObject *)this->parentObject());
  action_del_obj =
      new QAction(QString("Remove Object"), (QObject *)this->parentObject());
  action_edit = new QAction(QString("Edit"), (QObject *)this->parentObject());
  action_compress_obj = new QAction(QString("Compress Track"), (QObject *)this->parentObject());
  action_goto_first = new QAction(QString("Jump to first"), (QObject *)this->parentObject());
  action_goto_last = new QAction(QString("Jump to last"), (QObject *)this->parentObject());


  QObject::connect(action_del, SIGNAL(triggered()), this,
                   SLOT(removeAnnotation()));
  QObject::connect(action_del_obj, SIGNAL(triggered()), this,
                   SLOT(removeObject()));
  QObject::connect(action_edit, SIGNAL(triggered()), this,
                   SLOT(editAnnotation()));
  QObject::connect(action_compress_obj, SIGNAL(triggered()), this,
                     SLOT(compressObject()));
  QObject::connect(action_goto_first, SIGNAL(triggered()), this,
                     SLOT(goToFirst()));
  QObject::connect(action_goto_last, SIGNAL(triggered()), this,
                     SLOT(goToLast()));
}

AnnotationGraphicsItem::~AnnotationGraphicsItem() {
  for (int i = 0; i < 4; ++i) {
    delete corners[i];
  }

  //delete actions
  delete action_del;
  delete action_edit;
  delete action_del_obj;
  delete action_goto_first;
  delete action_goto_last;
}

bool AnnotationGraphicsItem::isAnnotationSelected() const {
  return this->getAnnotation() == AnnotationGraphicsItem::getSelectedAnnotation();
}

shared_ptr<AnnotatorLib::Annotation> AnnotationGraphicsItem::getAnnotation() const {
  return annotation;
}

QColor AnnotationGraphicsItem::idToColor(long id) const {
  static double r_ = (1 + sqrt(5)) / 2;
  static double g_ = (3 + sqrt(7)) / 2;
  static double b_ = (11 + sqrt(13)) / 2;

  double r = id * r_ - floor(id * r_);
  double g = id * g_ - floor(id * g_);
  double b = id * b_ - floor(id * b_);
  return QColor(r * 255, g * 255, b * 255);
}

void AnnotationGraphicsItem::setPlayer(Player *player) {
  this->player = player;
}

void AnnotationGraphicsItem::initCorners() {
  for (int i = 0; i < 4; ++i) {
    if (this->width < 20 || this->height < 20)
      corners[i] = new Corner(this, i, 6);
    else
      corners[i] = new Corner(this, i);
    corners[i]->hide();
    // corners[i]->installSceneEventFilter(this);
  }
  setCornerPositions();
}

void AnnotationGraphicsItem::initIdText() {
  if (annotation) {
    idText.setParentItem(this);
    idText.setDefaultTextColor(Qt::black);
    idText.setPlainText(QString::number(annotation->getId()));

    idText.setPos(rectX, rectY - 20);
    idText.hide();
  }
}

void AnnotationGraphicsItem::highlight( const int reason) {

  switch(reason) {
    case 0: //hovered
      borderColor = Qt::yellow;
      brush = getGradient();
      break;
    default:
      hideHighlight();
      return;
  }

  for (int i = 0; i < 4; ++i) {
    corners[i]->show();
    corners[i]->installSceneEventFilter(this);
  }

  setCornerPositions();
  pen.setColor(borderColor);

  brush = getGradient();
  idText.show();
  update();
}

void AnnotationGraphicsItem::hideHighlight( ) {

  for (int i = 0; i < 4; ++i) {
    corners[i]->hide();
  }

  if (isAnnotationSelected())
    borderColor = Qt::white;
  else
    borderColor = originColor;

  brush = Qt::NoBrush;

  setCornerPositions();
  pen.setColor(borderColor);

  idText.show();
  update();
}

void AnnotationGraphicsItem::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseDoubleClickEvent(event);
  if ( annotation.get() != nullptr ) {
      AnnotationGraphicsItem::setSelectedAnnotation(this->annotation);
      hideHighlight();
      if (AnnotationGraphicsItem::selected_annotation_item)
        AnnotationGraphicsItem::selected_annotation_item->hideHighlight(); //hide previous selection
      AnnotationGraphicsItem::selected_annotation_item = this;
      this->player->selectObject(this->annotation->getObject());
  }
}

/**
 * Will be triggered when user presses right
 *
*/
void AnnotationGraphicsItem::contextMenuEvent(
    QGraphicsSceneContextMenuEvent *event) {

  this->player->pause();

  // init context menu
  QMenu contextMenu("Context menu");
  contextMenu.addAction(action_edit);

  if (!this->annotation->isTemporary()) {
    contextMenu.addAction(
        action_del); // you cannot delete a temporary annotation
  }

  contextMenu.addAction(action_del_obj);
  contextMenu.addAction(action_goto_first);
  contextMenu.addAction(action_goto_last);
  contextMenu.addAction(action_compress_obj);

  action_edit->setData(event->screenPos());
  contextMenu.exec(event->screenPos());
}

void AnnotationGraphicsItem::removeObject() {

  shared_ptr<AnnotatorLib::Commands::RemoveObject> cmd =
      std::make_shared<AnnotatorLib::Commands::RemoveObject>(player->getSession(), annotation->getObject());
  CommandController::instance()->execute(cmd);
}

void AnnotationGraphicsItem::goToLast() {
  player->jumpTo(annotation->getObject()->getLastAnnotation()->getFrame()->getFrameNumber());
}

void AnnotationGraphicsItem::goToFirst() {
  player->jumpTo(annotation->getObject()->getFirstAnnotation()->getFrame()->getFrameNumber());
}

void AnnotationGraphicsItem::compressObject() {

  shared_ptr<AnnotatorLib::Object> obj = annotation->getObject();
  shared_ptr<AnnotatorLib::Commands::CompressObject> cmd =
      std::make_shared<AnnotatorLib::Commands::CompressObject>(player->getSession(),
                                                   obj);
  int nmb_annotations_before = obj->getAnnotations().size();
  CommandController::instance()->execute(cmd);
  int nmb_annotations_after = obj->getAnnotations().size();
  Alert msgBox;
  msgBox.setText(QString::fromStdString(std::string("Compression algorithm has removed ")
                                        + std::to_string(nmb_annotations_before - nmb_annotations_after)
                                        + std::string(" annotations.\n")));
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setStandardButtons(0);
  msgBox.setAutoClose(true);
  msgBox.setTimeout(3); //Closes after three seconds
  msgBox.exec();
}

void AnnotationGraphicsItem::removeAnnotation() {
  // TODO: remove rect permanently (works nnot everytime)
  shared_ptr<AnnotatorLib::Commands::RemoveAnnotation> cmd =
      std::make_shared<AnnotatorLib::Commands::RemoveAnnotation>(player->getSession(),
                                                   annotation);
  CommandController::instance()->execute(cmd);
}

void AnnotationGraphicsItem::editAnnotation() {

  QPoint posFromAction = action_edit->data().toPoint();

  EditObjectDialog editObjectDialog(player->getSession(),
                                    annotation->getObject());
  editObjectDialog.move(posFromAction.x(), posFromAction.y());
  editObjectDialog.exec();
}

/////////////////////////////////////////////////////////////
/**
 * change item setting: if mouse on hover
*/
void AnnotationGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  highlight(0);
}

/**
 * change item setting: if mouse on hover
*/
void AnnotationGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  hideHighlight();
}

/**
 * set corner position
*/
void AnnotationGraphicsItem::setCornerPositions() {
  corners[0]->setPos(rectX, rectY);
  corners[1]->setPos(rectX + width - corners[0]->getWidth(), rectY);
  corners[2]->setPos(rectX + width - corners[0]->getWidth(),
                     rectY + height - corners[0]->getHeight());
  corners[3]->setPos(rectX, rectY + height - corners[0]->getHeight());
}

void AnnotationGraphicsItem::getCornerPositions(Corner *corner, qreal x,
                                                qreal y) {
  // XSign and YSign to add add or substract size of Item

  int XSign = 0;
  int YSign = 0;
  switch (corner->getCorner()) {
  case 0: {
    XSign = +1;
    YSign = +1;
  } break;

  case 1: {
    XSign = -1;
    YSign = +1;
  } break;

  case 2: {
    XSign = -1;
    YSign = -1;
  } break;

  case 3: {
    XSign = +1;
    YSign = -1;
  } break;
  default: {}
  }

  // Set the new size of Item, whene the the corner position is changed.
  int XPos = corner->mouseDownX - x;
  int YPos = corner->mouseDownY - y;

  // set min width and min height to 10 px.
  int newWidth = width + (XSign * XPos);
  if (newWidth < 10)
    newWidth = 10;

  int newHeight = height + (YSign * YPos);
  if (newHeight < 10)
    newHeight = 10;

  int deltaWidth = newWidth - width;
  int deltaHeight = newHeight - height;

  // set the new size to item.
  width += deltaWidth;
  height += deltaHeight;

  deltaWidth *= (-1);
  deltaHeight *= (-1);

  switch (corner->getCorner()) {
  case 0: {
    int newXPos = this->pos().x() + deltaWidth;
    int newYpos = this->pos().y() + deltaHeight;
    this->setPos(newXPos, newYpos);
  } break;

  case 1: {
    int newYpos = this->pos().y() + deltaHeight;
    this->setPos(this->pos().x(), newYpos);
  } break;

  case 2: {
    this->setPos(this->pos().x(), this->pos().y());
  } break;

  case 3: {
    int newXPos = this->pos().x() + deltaWidth;
    this->setPos(newXPos, this->pos().y());
  } break;
  default:
    break;
  }
}

QBrush AnnotationGraphicsItem::getGradient() {
  QLinearGradient gradient;
  gradient.setStart(rectX, rectY);
  gradient.setFinalStop(width, height);

  QColor grey1(150, 150, 150, 125);
  QColor grey2(225, 225, 225, 125);

  gradient.setColorAt((qreal)0, grey1);
  gradient.setColorAt((qreal)1, grey2);
  return gradient;
}

/**
 * filter event: to get mouse press, move and release from parent graphicsScene
*/
bool AnnotationGraphicsItem::sceneEventFilter(QGraphicsItem *watched,
                                              QEvent *event) {
  Corner *corner = dynamic_cast<Corner *>(watched);
  QGraphicsSceneMouseEvent *own_event =
      dynamic_cast<QGraphicsSceneMouseEvent *>(event);

  if (corner && own_event) {
    switch (event->type()) {
    // if the mouse pressed, save igh (x,y) coordinates inside the corner
    // object
    case QEvent::GraphicsSceneMousePress: {
      corner->setMouseState(Corner::MouseDown);
      corner->mouseDownX = own_event->pos().x();
      corner->mouseDownY = own_event->pos().y();
      //this->setSelected(true);
    } break;

    case QEvent::GraphicsSceneMouseRelease: {
      corner->setMouseState(Corner::MouseReleased);
      changeAnnotationSize((int)this->x(), (int)this->y(), (int)this->width,
                           (int)this->height);
      return true;
    } break;

    case QEvent::GraphicsSceneMouseMove: {
      corner->setMouseState(Corner::MouseMoving);
      qreal x = own_event->pos().x();
      qreal y = own_event->pos().y();
      getCornerPositions(corner, x, y);
      setCornerPositions();

      this->update();
    } break;

    default:
      return false;
      break;
    }

    setCornerPositions();

    this->update();
  }

  return true;
}

void AnnotationGraphicsItem::mouseReleaseEvent(
    QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);

  if (event->button() == Qt::LeftButton) {
    changeAnnotationPosition(this->x(), this->y());
  }
}

void AnnotationGraphicsItem::changeAnnotationPosition(int x, int y) {
  if (x == annotation->getX() && y == annotation->getY())
    return;

  changeAnnotationSize(x, y, annotation->getWidth(), annotation->getHeight());
}

void AnnotationGraphicsItem::changeAnnotationSize(int x, int y, int w, int h) {

  shared_ptr<AnnotatorLib::Commands::Command> nA;
  if (annotation->isTemporary()) {
    nA = std::make_shared<AnnotatorLib::Commands::NewAnnotation>(
          player->getSession(), annotation->getObject(), annotation->getFrame(), x, y, w, h);
  } else {
    nA = std::make_shared<AnnotatorLib::Commands::UpdateAnnotation>(annotation, x, y, w, h);
  }
  CommandController::instance()->execute(nA);

  // update position of last annotation in automation plugin
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  if (plugin) {
    // plugin->setObject(annotation->getObject());
    // plugin->setLastAnnotation(annotation);
  }
}
