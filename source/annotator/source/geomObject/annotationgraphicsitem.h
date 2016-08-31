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
    AnnotationGraphicsItem() = delete;
    AnnotationGraphicsItem(AnnotatorLib::Annotation *annotation);
    virtual ~AnnotationGraphicsItem();
    AnnotatorLib::Annotation *getAnnotation();


    QColor idToColor(long id);

    void setPlayer(Player *player);

signals:
    void objectSelected(AnnotatorLib::Object *object);

protected:
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
    Corner*  corners[4];

protected:
    QBrush getGradient();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:

    static AnnotatorLib::Annotation* sel_annotation;
    AnnotatorLib::Annotation *annotation;

    void setCornerPositions();
    void getCornerPositions(Corner *corner, qreal x, qreal y);
    void changeAnnotationPosition(int x, int y);
    void changeAnnotationSize(int x, int y, int w, int h);
    void initCorners();
    void initIdText();

    /**
     * @brief hightLight
     * show id and corners
     */
    void highLight();

    /**
     * @brief hide
     * hide id and corners (if highlighted)
     */
    void hide();

    void showContextMenu(const QPoint &pos);

private slots:
    void removeAnnotation();        //slot
    void editAnnotation();          //slot

};

#endif // ANNOTATIONGRAPHICSITEM_H
