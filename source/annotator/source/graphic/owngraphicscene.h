#ifndef OWNGRAPHICSCENE_H
#define OWNGRAPHICSCENE_H

#include <AnnotatorLib/Session.h>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QImage>
#include <QPainter>

#define DrawON 0
#define DrawOFF 1

class OwnGraphicScene : public QGraphicsScene {
  Q_OBJECT
 public:
  explicit OwnGraphicScene();
  void addBackgroundImg(QImage img);

  QPoint inputCoordinate;  // image/video coordinate

  void setCurrentFrame(int frame);
  void setSession(AnnotatorLib::Session *session);

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  virtual void drawBackground(QPainter *painter, const QRectF &rect);

 private slots:
  // void on_btnPause_clicked();
  void on_objectSelected(AnnotatorLib::Object *object);

 signals:
  void on_btnPause_clicked();

 protected:
  unsigned long currentFrame = 0;
  AnnotatorLib::Session *session = nullptr;
  AnnotatorLib::Object *selected_obj = nullptr;

 private:
  QImage image;
  int drawMode;

  QGraphicsRectItem *selectionRect = nullptr;

  QPointF point1;  // clicked point
  QPointF point2;  // released point

  bool isDrawn;
  bool isLeftPressed;  // left click is clicked
  bool isItemMove;     // if item was moving

  QColor itemColor;

  void rectDraw(QPointF pt1, QPointF pt2);

  QPoint adjustCoordinate(QPointF MousePos);
  QPointF resetCoordinate(QPointF RectPos);
};

#endif  // OWNGRAPHICSCENE_H
