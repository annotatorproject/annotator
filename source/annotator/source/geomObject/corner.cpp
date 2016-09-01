#include "corner.h"

Corner::Corner(QGraphicsItem *parent, int corner)
    : QGraphicsItem(parent),
      cornerborderColor(Qt::green),
      cornerborderPen(),
      cornerwidth(10),
      cornerheight(10),
      corner(corner),
      mouseButtonState(MouseReleased) {
  setParentItem(parent);

  cornerborderPen.setWidth(2);
  cornerborderPen.setColor(cornerborderColor);

  this->setAcceptHoverEvents(true);
}

void Corner::setMouseState(int s) { mouseButtonState = s; }

int Corner::getMouseState() { return mouseButtonState; }

int Corner::getWidth() { return cornerwidth; }

int Corner::getHeight() { return cornerheight; }

int Corner::getCorner() { return corner; }

// change the color of captured object on hover events

void Corner::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  cornerborderColor = Qt::green;
  this->update(0, 0, cornerwidth, cornerheight);
}

void Corner::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  cornerborderColor = Qt::red;
  this->update(0, 0, cornerwidth, cornerheight);
}

QRectF Corner::boundingRect() const {
  return QRectF(0, 0, cornerwidth, cornerheight);
}

void Corner::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                   QWidget *) {
  // fill the box with color

  cornerborderPen.setCapStyle(Qt::SquareCap);
  cornerborderPen.setStyle(Qt::SolidLine);
  painter->setPen(cornerborderPen);

  QRectF rect(0, 0, cornerwidth, cornerheight);

  QBrush brush(Qt::SolidPattern);
  brush.setColor(cornerborderColor);
  painter->fillRect(rect, brush);
}
