#include "rectangleannotation.h"

RectangleAnnotation::RectangleAnnotation(AnnotatorLib::Annotation *annotation)
    : AnnotationGraphicsItem::AnnotationGraphicsItem(annotation) {
  assert(annotation->getObject());
  init();
}

void RectangleAnnotation::init() {
  setAcceptHoverEvents(true);
  setFlag(ItemIsMovable, true);
  setFlag(ItemIsSelectable, true);

  // idText->hide();

  if (this->getAnnotation()->isInterpolated()) pen.setStyle(Qt::DashLine);

  pen.setColor(borderColor);
  pen.setWidth(2);
}

/**
 * define item as bounding rectangle
*/
QRectF RectangleAnnotation::boundingRect() const {
  return QRectF(rectX, rectY, width, height);
}

/**
 * paint: draw item on parent scene
*/
void RectangleAnnotation::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget) {
  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawRect(boundingRect());
}
