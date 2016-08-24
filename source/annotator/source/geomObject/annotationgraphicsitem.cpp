#include <QMenu>
#include <QObject>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Commands/RemoveAnnotation.h>

#include "annotationgraphicsitem.h"
#include "plugins/pluginloader.h"

AnnotatorLib::Annotation *AnnotationGraphicsItem::getAnnotation() {
  return annotation;
}

AnnotationGraphicsItem::AnnotationGraphicsItem(
    AnnotatorLib::Annotation *annotation)
    : QGraphicsItem::QGraphicsItem(nullptr) {
  this->annotation = annotation;

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
  cornerXPos = 0;
  cornerYPos = 0;
  cornerWidth = width, cornerHeight = height,

  originColor = borderColor;
}

AnnotationGraphicsItem::~AnnotationGraphicsItem() {
  for (int i = 0; i < 4; ++i) {
    delete corners[i];
  }
}

static double r_ = (1 + sqrt(5)) / 2;
static double g_ = (3 + sqrt(7)) / 2;
static double b_ = (11 + sqrt(13)) / 2;

QColor AnnotationGraphicsItem::idToColor(long id) {
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
  idText.setParentItem(this);
  idText.setDefaultTextColor(borderColor);
  idText.setPlainText(QString::number(annotation->getId()));

  idText.setPos(rectX, rectY - 20);
  idText.hide();
}

void AnnotationGraphicsItem::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseDoubleClickEvent(event);
  if (this->player != nullptr && this->annotation != nullptr &&
      this->annotation->getObject() != nullptr) {
    this->player->selectObject(annotation->getObject());
  }
}

/**
 * Will be triggered when a
 *
*/
void AnnotationGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //this->player->pause();
    showContextMenu( event->screenPos());
}

void AnnotationGraphicsItem::showContextMenu(const QPoint &pos)
{
   QMenu contextMenu("Context menu");

   QAction action_del(QString("Remove"), (QObject*)this->parentObject());
   QAction action_edit(QString("Edit"), (QObject*)this->parentObject());

   QObject::connect(&action_del, SIGNAL(triggered()), this, SLOT(removeAnnotation()));
   QObject::connect(&action_edit, SIGNAL(triggered()), this, SLOT(editAnnotation()));

   contextMenu.addAction(&action_del);
   contextMenu.addAction(&action_edit);
   contextMenu.exec(pos);
}

/*
void AnnotationGraphicsItem::changeAnnotationPosition(int x, int y)
{
    annotation->setPosition(x, y);
}*/


void AnnotationGraphicsItem::removeAnnotation()
{
    //TODO: remove rect permanently
    AnnotatorLib::Commands::RemoveAnnotation * cmd = new AnnotatorLib::Commands::RemoveAnnotation(player->getSession(), annotation);
    player->getSession()->execute(cmd);
}


void AnnotationGraphicsItem::editAnnotation()
{
    //TODO: emit signal to show popup
}

/////////////////////////////////////////////////////////////
/**
 * change item setting: if mouse on hover
*/
void AnnotationGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  corners[0] = new Corner(this, 0);
  corners[1] = new Corner(this, 1);
  corners[2] = new Corner(this, 2);
  corners[3] = new Corner(this, 3);

  corners[0]->installSceneEventFilter(this);
  corners[1]->installSceneEventFilter(this);
  corners[2]->installSceneEventFilter(this);
  corners[3]->installSceneEventFilter(this);

  setCornerPositions();

  borderColor = Qt::green;

  QLinearGradient gradient;
  gradient.setStart(cornerXPos, cornerYPos);
  gradient.setFinalStop(cornerWidth, cornerYPos);

  QColor grey1(150, 150, 150, 125);
  QColor grey2(225, 225, 225, 125);

  gradient.setColorAt((qreal)0, grey1);
  gradient.setColorAt((qreal)1, grey2);

  brush = gradient;
  idText.show();

  update();
}

/**
 * set corner position
*/
void AnnotationGraphicsItem::setCornerPositions() {
  corners[0]->setPos(cornerXPos, cornerYPos);
  corners[1]->setPos(cornerXPos + cornerWidth - corners[0]->getWidth(),
                     cornerYPos);
  corners[2]->setPos(cornerXPos + cornerWidth - corners[0]->getWidth(),
                     cornerYPos + cornerHeight - corners[0]->getHeight());
  corners[3]->setPos(cornerXPos,
                     cornerYPos + cornerHeight - corners[0]->getHeight());
}

/**
 * reset item setting: if mouse leave hover
*/
void AnnotationGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  corners[0]->setParentItem(NULL);
  corners[1]->setParentItem(NULL);
  corners[2]->setParentItem(NULL);
  corners[3]->setParentItem(NULL);

  delete corners[0];
  delete corners[1];
  delete corners[2];
  delete corners[3];

  borderColor = originColor;

  brush = Qt::NoBrush;
  idText.hide();
  update();
}

/**
 * set new size to the item
*/
void AnnotationGraphicsItem::setSize(int x, int y) {
  width += x;
  height += y;

  cornerWidth = width;
  cornerHeight = height;
}

/**
 * filter event: to get mouse press, move and release from parent graphicsScene
*/
bool AnnotationGraphicsItem::sceneEventFilter(QGraphicsItem *watched,
                                              QEvent *event) {
  Corner *corner = dynamic_cast<Corner *>(watched);
  if (corner == NULL) return false;

  QGraphicsSceneMouseEvent *own_event =
      dynamic_cast<QGraphicsSceneMouseEvent *>(event);
  if (own_event == NULL) {
    return false;
  }

  switch (event->type()) {
    // if the mouse pressed, save the (x,y) coordinates inside the corner object
    case QEvent::GraphicsSceneMousePress: {
      corner->setMouseState(Corner::MouseDown);
      corner->mouseDownX = own_event->pos().x();
      corner->mouseDownY = own_event->pos().y();
      this->setSelected(true);
    } break;

    case QEvent::GraphicsSceneMouseRelease: {
      corner->setMouseState(Corner::MouseReleased);
      changeAnnotationSize((int)this->x(), (int)this->y(), (int)this->width,
                           (int)this->height);
    } break;

    case QEvent::GraphicsSceneMouseMove: {
      corner->setMouseState(Corner::MouseMoving);
    } break;

    default:
      return false;
      break;
  }

  if (corner->getMouseState() == Corner::MouseMoving) {
    qreal x = own_event->pos().x();
    qreal y = own_event->pos().y();

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
    setSize(deltaWidth, deltaHeight);

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

    setCornerPositions();

    this->update();
  }

  return true;
}

void AnnotationGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);

  if(event->button() ==  Qt::LeftButton)
  {
      deltax = this->x();
      deltay = this->y();

  }
}

void AnnotationGraphicsItem::mouseReleaseEvent(
    QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);

  if(event->button() ==  Qt::LeftButton)
  {
    deltax = this->x() - deltax;
    deltay = this->y() - deltay;

    changeAnnotationPosition(this->x(), this->y());
    setCornerPositions();
    this->update();

  }
}

void AnnotationGraphicsItem::setAnnotationSize(int x, int y) {
  annotation->setPosition(annotation->getX(), annotation->getY(), x, y);
}

void AnnotationGraphicsItem::changeAnnotationPosition(int x, int y) {
  if (annotation->isInterpolated()) {
    annotation->setInterpolated(false);
    AnnotatorLib::Commands::NewAnnotation *nA =
        new AnnotatorLib::Commands::NewAnnotation(
            annotation->getObject(), annotation->getFrame(), x, y,
            annotation->getWidth(), annotation->getHeight(),
            annotation->getNext(), annotation->getPrevious(),
            player->getSession(), true);
    player->getSession()->execute(nA);
  } else {
    AnnotatorLib::Commands::UpdateAnnotation *uA =
        new AnnotatorLib::Commands::UpdateAnnotation(
            annotation, x, y, annotation->getWidth(), annotation->getHeight());
    player->getSession()->execute(uA);
  }

  // update position of last annotation in automation plugin
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  if (plugin) {
    plugin->setObject(annotation->getObject());
    plugin->setLastAnnotation(annotation);
  }
}

void AnnotationGraphicsItem::changeAnnotationSize(int x, int y, int w, int h) {
  if (annotation->isInterpolated()) {
    annotation->setInterpolated(false);
    AnnotatorLib::Commands::NewAnnotation *nA =
        new AnnotatorLib::Commands::NewAnnotation(
            annotation->getObject(), annotation->getFrame(), x, y, w, h,
            annotation->getNext(), annotation->getPrevious(),
            player->getSession(), true);
    player->getSession()->execute(nA);
  } else {
    AnnotatorLib::Commands::UpdateAnnotation *uA =
        new AnnotatorLib::Commands::UpdateAnnotation(annotation, x, y, w, h);
    player->getSession()->execute(uA);
  }

  // update position of last annotation in automation plugin
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getCurrent();
  if (plugin) {
    plugin->setObject(annotation->getObject());
    plugin->setLastAnnotation(annotation);
  }
}
