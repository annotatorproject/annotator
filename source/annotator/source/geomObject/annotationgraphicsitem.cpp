#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/RemoveAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <QMenu>
#include <QObject>

#include "annotationgraphicsitem.h"
#include "controller/commandcontroller.h"
#include "gui/newannotationdialog.h"
#include "plugins/pluginloader.h"


//static
AnnotatorLib::Annotation* selected_annotation = nullptr;

AnnotatorLib::Annotation *AnnotationGraphicsItem::getAnnotation() {
  return annotation;
}

AnnotationGraphicsItem::AnnotationGraphicsItem(
    AnnotatorLib::Annotation *annotation)
    : QGraphicsItem::QGraphicsItem(nullptr), annotation(annotation) {
  assert(annotation);
  assert(annotation->getObject());

  if (annotation->getObject() == nullptr)
    borderColor = idToColor(annotation->getId());
  else
    borderColor = idToColor(annotation->getObject()->getId());

  setPos(annotation->getX(), annotation->getY());
  initCorners();
  initIdText();

  rectX = 0;
  rectY = 0;
  width = annotation->getWidth();
  height = annotation->getHeight();
  originColor = borderColor;
}

AnnotationGraphicsItem::~AnnotationGraphicsItem() {
  for (int i = 0; i < 4; ++i) {
    delete corners[i];
  }
  if (annotation->isInterpolated())
    delete annotation;
}

QColor AnnotationGraphicsItem::idToColor(long id) {
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
    corners[i] = new Corner(this, i);
    corners[i]->hide();
    // corners[i]->installSceneEventFilter(this);
  }
  setCornerPositions();
}

void AnnotationGraphicsItem::initIdText() {
  if (annotation) {
    idText.setParentItem(this);
    idText.setDefaultTextColor(borderColor);
    idText.setPlainText(QString::number(annotation->getId()));

    idText.setPos(rectX, rectY - 20);
    idText.hide();
  }
}

void AnnotationGraphicsItem::highLight() {
  for (int i = 0; i < 4; ++i) {
    corners[i]->setVisible(true);
    corners[i]->installSceneEventFilter(this);
  }

  borderColor = Qt::green;

  brush = getGradient();
  idText.show();
  setCornerPositions();
}

void AnnotationGraphicsItem::hide() {
  for (int i = 0; i < 4; ++i) {
    corners[i]->hide();
  }
  borderColor = originColor;

  brush = Qt::NoBrush;
  idText.hide();
  update();
}

void AnnotationGraphicsItem::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseDoubleClickEvent(event);
  if (this->player != nullptr &&
      this->annotation != nullptr &&
      this->annotation->getObject() != nullptr) {

      this->player->selectObject(annotation->getObject());
  }
}

/**
 * Will be triggered when a
 *
*/
void AnnotationGraphicsItem::contextMenuEvent(
    QGraphicsSceneContextMenuEvent *event) {
  // this->player->pause();
  showContextMenu(event->screenPos());
}

void AnnotationGraphicsItem::showContextMenu(const QPoint &pos) {
  QMenu contextMenu("Context menu");

  QAction action_del(QString("Remove"), (QObject *)this->parentObject());
  QAction action_edit(QString("Edit"), (QObject *)this->parentObject());

  QObject::connect(&action_del, SIGNAL(triggered()), this,
                   SLOT(removeAnnotation()));
  QObject::connect(&action_edit, SIGNAL(triggered()), this,
                   SLOT(editAnnotation()));

  if (!this->annotation->isInterpolated()) {
      contextMenu.addAction(&action_del);  //you cannot delete a temporary annotation
  }
  contextMenu.addAction(&action_edit);
  contextMenu.exec(pos);
}

void AnnotationGraphicsItem::removeAnnotation() {
  // TODO: remove rect permanently (works nnot everytime)
  AnnotatorLib::Commands::RemoveAnnotation *cmd =
      new AnnotatorLib::Commands::RemoveAnnotation(player->getSession(),
                                                   annotation);
  CommandController::instance()->execute(cmd);
}

void AnnotationGraphicsItem::editAnnotation() {

  if (this->annotation->isInterpolated()) {
      //TODO: create a new annotation
  }
  // TODO: show dialog
}

/////////////////////////////////////////////////////////////
/**
 * change item setting: if mouse on hover
*/
void AnnotationGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  highLight();
  update();
}

/**
 * change item setting: if mouse on hover
*/
void AnnotationGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  hide();
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
  if (newWidth < 10) newWidth = 10;

  int newHeight = height + (YSign * YPos);
  if (newHeight < 10) newHeight = 10;

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
      // if the mouse pressed, save the (x,y) coordinates inside the corner
      // object
      case QEvent::GraphicsSceneMousePress: {
        corner->setMouseState(Corner::MouseDown);
        corner->mouseDownX = own_event->pos().x();
        corner->mouseDownY = own_event->pos().y();
        // this->setSelected(true);
      } break;

      case QEvent::GraphicsSceneMouseRelease: {
        corner->setMouseState(Corner::MouseReleased);
        changeAnnotationSize((int)this->x(), (int)this->y(), (int)this->width,
                             (int)this->height);
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
  if (x == annotation->getX() && y == annotation->getY()) return;

  changeAnnotationSize(x, y, annotation->getWidth(), annotation->getHeight());
}

void AnnotationGraphicsItem::changeAnnotationSize(int x, int y, int w, int h) {

  AnnotatorLib::Commands::Command *nA;
  if (annotation->isInterpolated()) {
    nA = new AnnotatorLib::Commands::NewAnnotation(
            annotation->getObject(), annotation->getFrame(), x, y, w, h,
            player->getSession(), true);
  } else {
    nA = new AnnotatorLib::Commands::UpdateAnnotation(annotation, x, y, w, h);

  }
  CommandController::instance()->execute(nA);

  // update position of last annotation in automation plugin
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  if (plugin) {
    //plugin->setObject(annotation->getObject());
    //plugin->setLastAnnotation(annotation);
  }
}
