#ifndef OWNGRAPHICVIEW_H
#define OWNGRAPHICVIEW_H

#include "owngraphicscene.h"

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QPointF>
#include <QRectF>
#include <QDebug>


#define onDRAW   0
#define offDRAW  1

class OwnGraphicScene;

class OwnGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    OwnGraphicView(QGraphicsScene *scene, QWidget *parent = 0);

    void setBackground(QImage background);
    void updateSceneSize(int width, int height);

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
