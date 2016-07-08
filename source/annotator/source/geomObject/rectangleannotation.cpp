#include "rectangleannotation.h"

RectangleAnnotation::RectangleAnnotation(AnnotatorLib::Annotation *annotation):
    AnnotationGraphicsItem::AnnotationGraphicsItem(annotation)
{
    init();
}


void RectangleAnnotation::init()
{
    setAcceptHoverEvents(true);
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);

    //idText->hide();
}


/**
 * define item as bounding rectangle
*/
QRectF RectangleAnnotation::boundingRect() const
{
    return QRectF(rectX,rectY,cornerWidth,cornerHeight);
}



/**
 * paint: draw item on parent scene
*/
void RectangleAnnotation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    pen.setColor(borderColor);
    pen.setWidth(2);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawRect(boundingRect());
}





