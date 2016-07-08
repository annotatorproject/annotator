#ifndef OWNGRAPHICSCENE_H
#define OWNGRAPHICSCENE_H

#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "geomObject/ownqgaphicsItem.h"
#include "interpolation/popup.h"


#define DrawON   0
#define DrawOFF  1

class OwnGraphicScene:public QGraphicsScene
{
    Q_OBJECT
public:
    explicit OwnGraphicScene(Popup *parent);
    void addBackgroundImg(QImage img);

    QPoint inputCoordinate;     //image/video coordinate
    OwnQGraphicsItem* rectangle;
    QString id_File;

    QList<OwnQGraphicsItem*> ItemsList;
    void setItem(QPointF point_Top, QPointF point_Bottom, int id, QColor borderColor);

    OwnQGraphicsItem *getItemByID(QString ID);
    void clearItemsList();
    void editItem(OwnQGraphicsItem *rectangle);



public slots:
    void removeLastItem();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private slots:

    void saveIDsettings();

signals:
    void on_btnPause_clicked();

private:
    Popup *popup;
    QImage image;
    int drawMode;
    QPainter* painter;

    QPointF point1;     //clicked point
    QPointF point2;     //released point


    bool isDrawn;
    bool isLeftPressed; //left click is clicked
    bool isItemMove;    //if item was moving

    QColor itemColor;

    void rectDraw(QPointF pt1,QPointF pt2);


    int loadSettings();
    QPoint adjustCoordinate(QPointF MousePos);
    QPointF resetCoordinate(QPointF RectPos);
};

#endif // OWNGRAPHICSCENE_H
