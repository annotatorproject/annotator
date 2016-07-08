#include "corner.h"

Corner::Corner(QGraphicsItem *parent,  int corner) :
    QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    cornerborderColor(Qt::green),
    cornerborderPen(),
    cornerwidth(10),
    cornerheight(10),
    corner(corner),
    mouseButtonState(MouseReleased)
{
    setParentItem(parent);

    cornerborderPen.setWidth(2);
    cornerborderPen.setColor(cornerborderColor);

   this->setAcceptHoverEvents(true);
}

void Corner::setMouseState(int s)
{
    mouseButtonState = s;
}

int Corner::getMouseState()
{
    return mouseButtonState;
}

int Corner::getWidth()
{
    return cornerwidth;
}

int Corner::getHeight()
{
    return cornerheight;
}

int Corner::getCorner()
{
    return corner;
}


// set accepted to false since are not actually handling them

void Corner::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void Corner::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void Corner::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
}

void Corner::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}

void Corner::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}


// change the color of captured object on hover events

void Corner::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    cornerborderColor = Qt::black;
    this->update(0,0,cornerwidth,cornerheight);
}

void Corner::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    cornerborderColor = Qt::red;
    this->update(0,0,cornerwidth,cornerheight);
}

QRectF Corner::boundingRect() const
{
        return QRectF(0,0,cornerwidth,cornerheight);
}

void Corner::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // fill the box with color

    cornerborderPen.setCapStyle(Qt::SquareCap);
    cornerborderPen.setStyle(Qt::SolidLine);
    painter->setPen(cornerborderPen);

    QPointF topLeft (0, 0);
    QPointF bottomRight ( cornerwidth, cornerheight);

    QRectF rect (topLeft, bottomRight);

    QBrush brush (Qt::SolidPattern);
    brush.setColor (cornerborderColor);
    painter->fillRect(rect,brush);

}
