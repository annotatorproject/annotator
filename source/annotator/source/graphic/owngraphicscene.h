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
  void setBackground(QGraphicsPixmapItem *background);

  QPoint inputCoordinate; // image/video coordinate

  void setCurrentFrame(int frame);
  void setSession(std::shared_ptr<AnnotatorLib::Session> session);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  virtual void drawBackground(QPainter *painter, const QRectF &rect);

private slots:
  void on_signal_objectSelection(shared_ptr<AnnotatorLib::Object> object);

signals:
  void on_btnPause_clicked();

protected:
  unsigned long currentFrame = 0;
  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
  shared_ptr<AnnotatorLib::Object> selected_obj;

private:
  QImage image;
  int drawMode;

  QGraphicsRectItem *selectionRect = nullptr;

  QPointF point1; // clicked point
  QPointF point2; // released point

  bool isDrawn;
  bool isLeftPressed; // left click is clicked
  bool isItemMove;    // if item was moving

  QColor itemColor;

  void rectDraw(QPointF pt1, QPointF pt2);

  QPoint adjustCoordinate(QPointF MousePos);
  QPointF resetCoordinate(QPointF RectPos);

  /**
   * @brief background
   * the current image as scene background.
   */
  QGraphicsPixmapItem *background = nullptr;
};

#endif // OWNGRAPHICSCENE_H
