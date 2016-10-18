#ifndef CORNER_H
#define CORNER_H

#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPointF>

class Corner : public QGraphicsItem {
 public:
  explicit Corner(QGraphicsItem *parent = 0, int corner = 0, int size = 10);

  int getCorner();  //< allows the owner to find out which coner this is
  void setMouseState(
      int);             //< allows the owner to record the current mouse state
  int getMouseState();  //< allows the owner to get the current mouse state

  enum {
    MouseReleased = 0,
    MouseDown,
    MouseMoving
  };  ///< define the mouse states
  qreal mouseDownX;
  qreal mouseDownY;

  int getWidth();
  int getHeight();

 private:
  virtual QRectF boundingRect() const;  //< must be re-implemented in this class
                                        //to provide the diminsions of the box
                                        //to the QGraphicsView

  QColor cornerborderColor;  //< the hover event handlers will toggle this
                             //between red and black
  QPen cornerborderPen;      //< the pen is used to paint the red/black border

  qreal cornerwidth;
  qreal cornerheight;

  int corner;  // 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the
               // box
  int mouseButtonState;

 protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif  // CORNER_H
