#ifndef ANNOTATIONGRAPHICSITEM_H
#define ANNOTATIONGRAPHICSITEM_H

#include <AnnotatorLib/Annotation.h>
#include <AnnotatorLib/Object.h>

#include <QGraphicsItem>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsTextItem>

#include "player.h"
#include "corner.h"

class AnnotationGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    AnnotationGraphicsItem(AnnotatorLib::Annotation *annotation);
    virtual ~AnnotationGraphicsItem();
    AnnotatorLib::Annotation *getAnnotation();


    QColor idToColor(long id);

    void setPlayer(Player *player);
    AnnotatorLib::Annotation *annotation;
    void setSize(int x, int y);
signals:
    void objectSelected(AnnotatorLib::Object *object);

protected:
    //AnnotatorLib::Annotation *annotation;

    QPen pen;
    QBrush brush;
    QColor borderColor;
    QColor  originColor;
    QGraphicsTextItem idText;

    Player *player;

    qreal rectX;
    qreal rectY;
    qreal width;
    qreal height;
    qreal   cornerXPos;
    qreal   cornerYPos;
    qreal   cornerWidth;
    qreal   cornerHeight;
    Corner*  corners[4];

    int deltax = 0;
    int deltay = 0;

protected:
    void initCorners();
    void initIdText();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    void setCornerPositions();
    void setAnnotationSize(int x, int y);
    void changeAnnotationPosition(int x, int y);
    void changeAnnotationSize(int x, int y, int w, int h);

private:
    void showContextMenu(const QPoint &pos);

private slots:
    void removeAnnotation();        //slot
    void editAnnotation();          //slot

};

#endif // ANNOTATIONGRAPHICSITEM_H
