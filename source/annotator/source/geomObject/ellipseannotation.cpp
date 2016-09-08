#include "ellipseannotation.h"

EllipseAnnotation::EllipseAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation)
    : AnnotationGraphicsItem::AnnotationGraphicsItem(annotation) {
  init();
}

/*QRectF EllipseAnnotation::boundingRect() const
{
    return QRectF(0,0,
                  annotation->getHRadius()*2, annotation->getVRadius()*2);
}

void EllipseAnnotation::paint(QPainter *painter, const QStyleOptionGraphicsItem
*option, QWidget *widget)
{
    pen.setColor(borderColor);
    pen.setWidth(3);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(boundingRect());

}*/

void EllipseAnnotation::init() {
  setAcceptHoverEvents(true);
  setFlag(ItemIsMovable, true);
  setFlag(ItemIsSelectable, true);
}

/**
 * define item as bounding rectangle
*/
QRectF EllipseAnnotation::boundingRect() const {
  return QRectF(rectX, rectY, width, height);
}

/**
 * paint: draw item on parent scene
*/
void EllipseAnnotation::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget) {
  pen.setColor(borderColor);
  pen.setWidth(2);

  painter->setPen(pen);
  painter->setBrush(brush);

  painter->drawEllipse(boundingRect());
}
