#include "ownqgaphicsItem.h"
#include <QDebug>

OwnQGraphicsItem::OwnQGraphicsItem(qreal x, qreal y, qreal w, qreal h, QColor color):
    idText(),
    rectX(x),
    rectY(y),
    width(w),
    height(h),
    cornerXPos(x),
    cornerYPos(y),
    cornerWidth(width),
    cornerHeight(height),
    borderColor(color)
{
    //make item movable, selectable and hover
    this->setAcceptHoverEvents(true);
    this->setFlag(ItemIsMovable, true);
    this->setFlag(ItemIsSelectable, true);

    originColor = borderColor;

}


/**
 * define item as bounding rectangle
*/
QRectF OwnQGraphicsItem::boundingRect() const
{
    return QRectF(rectX,rectY,cornerWidth,cornerHeight);
}


/**
 * select item: change color and show idtext...
*/
void OwnQGraphicsItem::selectItem(OwnQGraphicsItem* item)
{
    item->borderColor = Qt::green;
    QLinearGradient gradient;
    gradient.setStart(cornerXPos,cornerYPos);
    gradient.setFinalStop( cornerWidth ,cornerYPos);

    QColor grey1(150,150,150,125);
    QColor grey2(225,225,225,125);


    gradient.setColorAt((qreal)0, grey1 );
    gradient.setColorAt((qreal)1, grey2 );

    item->brush = gradient;
    item->idText->show();
}

/**
 * diselect all: make item setting as default
*/
void OwnQGraphicsItem::diselectAll()
{
    borderColor = originColor;

    brush = Qt::NoBrush;
    idText->hide();
}


/**
 * change item setting: if mouse on hover
*/
void OwnQGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *){

    corners[0] = new Corner(this,0);
    corners[1] = new Corner(this,1);
    corners[2] = new Corner(this,2);
    corners[3] = new Corner(this,3);

    corners[0]->installSceneEventFilter(this);
    corners[1]->installSceneEventFilter(this);
    corners[2]->installSceneEventFilter(this);
    corners[3]->installSceneEventFilter(this);

    setCornerPositions();

    borderColor = Qt::green;

    QLinearGradient gradient;
    gradient.setStart(cornerXPos,cornerYPos);
    gradient.setFinalStop( cornerWidth ,cornerYPos);

    QColor grey1(150,150,150,125);
    QColor grey2(225,225,225,125);


    gradient.setColorAt((qreal)0, grey1 );
    gradient.setColorAt((qreal)1, grey2 );

    brush = gradient;
    idText->show();

    update();
}


/**
 * set corner position
*/
void OwnQGraphicsItem::setCornerPositions()
{

    corners[0]->setPos(cornerXPos, cornerYPos);
    corners[1]->setPos(cornerXPos+cornerWidth-corners[0]->getWidth(),  cornerYPos);
    corners[2]->setPos(cornerXPos+cornerWidth-corners[0]->getWidth() , cornerYPos+cornerHeight-corners[0]->getHeight());
    corners[3]->setPos(cornerXPos, cornerYPos+cornerHeight-corners[0]->getHeight());
}


/**
 * reset item setting: if mouse leave hover
*/
void OwnQGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *){
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
    idText->hide();
    update();
}


/**
 * set new size to the item
*/
void OwnQGraphicsItem::setSize(int x, int y)
{
    width += x;
    height += y;

    cornerWidth =  width;
    cornerHeight=  height;

}


/**
 * filter event: to get mpouse press, move and release from parent graphicsScene
*/
bool OwnQGraphicsItem::sceneEventFilter(QGraphicsItem * watched, QEvent * event )
{


    Corner * corner = dynamic_cast<Corner *>(watched);
    if ( corner == NULL) return false;

    QGraphicsSceneMouseEvent * own_event = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if ( own_event == NULL)
    {
        return false;
    }


    switch (event->type() )
    {
            // if the mouse pressed, save the (x,y) coordinates inside the corner object
        case QEvent::GraphicsSceneMousePress:
            {
                corner->setMouseState(Corner::MouseDown);
                corner->mouseDownX = own_event->pos().x();
                corner->mouseDownY = own_event->pos().y();
                this->setSelected(true);
            }
            break;

        case QEvent::GraphicsSceneMouseRelease:
            {
                corner->setMouseState(Corner::MouseReleased);

            }
            break;

        case QEvent::GraphicsSceneMouseMove:
            {
                corner->setMouseState(Corner::MouseMoving );
            }
            break;

        default:
            return false;
            break;
    }


    if ( corner->getMouseState() == Corner::MouseMoving )
    {

        qreal x = own_event->pos().x();
        qreal y = own_event->pos().y();

        //XSign and YSign to add add or substract size of Item

        int XSign = 0;
        int YSign = 0;
        switch( corner->getCorner() )
        {
        case 0:
            {
                XSign = +1;
                YSign = +1;
            }
            break;

        case 1:
            {
                XSign = -1;
                YSign = +1;
            }
            break;

        case 2:
            {
                XSign = -1;
                YSign = -1;
            }
            break;

        case 3:
            {
                XSign = +1;
                YSign = -1;
            }
            break;

        }


        //Set the new size of Item, whene the the corner position is changed.
        int XPos = corner->mouseDownX - x;
        int YPos = corner->mouseDownY - y;

        //set min width and min height to 10 px.
        int newWidth = width + ( XSign * XPos);
        if ( newWidth < 10 ) newWidth  = 10;

        int newHeight = height + (YSign * YPos) ;
        if ( newHeight < 10 ) newHeight = 10;

        int deltaWidth  = newWidth - width ;
        int deltaHeight = newHeight - height ;

        //set the new size to item.
        setSize(deltaWidth ,   deltaHeight);


        deltaWidth *= (-1);
        deltaHeight *= (-1);

        switch( corner->getCorner() )
        {
        case 0:
            {
                int newXPos = this->pos().x() + deltaWidth;
                int newYpos = this->pos().y() + deltaHeight;
                this->setPos(newXPos, newYpos);
            }
            break;

        case 1:
            {
                int newYpos = this->pos().y() + deltaHeight;
                this->setPos(this->pos().x(), newYpos);
            }
            break;

        case 2:
            {
                this->setPos(this->pos().x(), this->pos().y());
            }
            break;

        case 3:
            {
                int newXPos = this->pos().x() + deltaWidth;
                this->setPos(newXPos,this->pos().y());
            }
            break;

        }


        setCornerPositions();

        this->update();
    }

    return true;
}


/**
 * paint: draw item on parent scene
*/
void OwnQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    own_pen.setColor(borderColor);
    own_pen.setWidth(2);
    painter->setPen(own_pen);

    painter->setBrush(brush);

    QPointF topLeft2 (rectX, rectY);
    QPointF bottomRight2 ( rectX+cornerWidth, rectY+cornerHeight);

    QRectF rect2 (topLeft2, bottomRight2);

    painter->drawRect(rect2);

}


/**
 *  accept mouse release
*/
void OwnQGraphicsItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);

    QGraphicsItem::mouseReleaseEvent(event);
}


/**
 *  set new Id to the item
*/
void OwnQGraphicsItem::setID(int optional)
{
    idText = new QGraphicsTextItem(this);
    idText->setDefaultTextColor(borderColor);
    idText->setPos(rectX,rectY-20);
    idText->hide();

    if(optional != (-1)){
        idText->setPlainText(QString::number(optional));
        return;
    }
    idText->setPlainText(QString::number(9999));
}



/**
 *  get id from item
*/
QString OwnQGraphicsItem::getID(OwnQGraphicsItem *item)
{
    return item->idText->toPlainText();
}

QString OwnQGraphicsItem::getNextID()
{
    return this->idText->toPlainText();
}


/**
 *  get item upperLeft point
*/
QPointF OwnQGraphicsItem::getX(OwnQGraphicsItem *item)
{
    return item->mapToScene(item->boundingRect().topLeft());
}


/**
 *  get item LowerRight point
*/
QPointF OwnQGraphicsItem::getY(OwnQGraphicsItem *item)
{
    return item->mapToScene(item->boundingRect().bottomRight());
}


/**
 *  get item color
*/
QColor OwnQGraphicsItem::getColor(OwnQGraphicsItem *item)
{
    return item->originColor;
}


