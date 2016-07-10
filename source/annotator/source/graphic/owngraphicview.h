#ifndef OWNGRAPHICVIEW_H
#define OWNGRAPHICVIEW_H

#include <QGraphicsView>
#include "owngraphicscene.h"
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QPointF>
#include <QRectF>
#include <QDebug>
#include "geomObject/ownqgaphicsItem.h"


#define onDRAW   0
#define offDRAW  1

class OwnGraphicScene;
class OwnQGraphicsItem;

class OwnGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    OwnGraphicView(QGraphicsScene *scene, QWidget *parent = 0);

    void setBackground(QImage background);
    void updateSceneSize(int width, int height);


    OwnQGraphicsItem* rectangle;
    OwnGraphicScene* ownScene;

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
private:
    int drawMode;
    QPainter* painter;
    QPointF point1;
    QPointF point2;


};
#endif // OWNGRAPHICVIEW_H
