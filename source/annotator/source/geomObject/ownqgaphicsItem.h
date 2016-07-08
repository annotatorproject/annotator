#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include "corner.h"

#include <QBrush>
#include <QLinearGradient>
#include <QPen>

#include <QSettings>
#include <QApplication>


class OwnQGraphicsItem: public QGraphicsItem
{
public:
    explicit OwnQGraphicsItem(qreal x, qreal y, qreal w, qreal h, QColor color);
    virtual QRectF boundingRect() const;


    QGraphicsTextItem* idText;
    int id;


    void setID(int optional);


    QString getID(OwnQGraphicsItem *item);
    QString getNextID();

    QPointF getX(OwnQGraphicsItem *item);
    QPointF getY(OwnQGraphicsItem *item);

    QColor getColor(OwnQGraphicsItem *item);


    void selectItem(OwnQGraphicsItem *item);
    void diselectAll();

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);



    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:

    qreal rectX;
    qreal rectY;
    qreal width;
    qreal height;

    QPen own_pen;
    QBrush brush;

    QPointF location;
    QPointF startlocation;

    qreal   cornerXPos;
    qreal   cornerYPos;
    qreal   cornerWidth;
    qreal   cornerHeight;
    Corner*  corners[4];



    void setCornerPositions();
    void setSize(int x, int y);

    QColor  borderColor;
    QColor  originColor;
    QPainter *painter;


};

#endif // HIGHLIGHTITEM_H
