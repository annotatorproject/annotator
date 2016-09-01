#include "owngraphicscene.h"
#include "graphic/owngraphicscene.h"
#include "gui/newannotationdialog.h"

#include <algorithm>

#include <QDebug>
#include <QSettings>

OwnGraphicScene::OwnGraphicScene()
    : isDrawn(false), isLeftPressed(false), isItemMove(false) {}

/**
 * set background image to scene
*/
void OwnGraphicScene::addBackgroundImg(QImage img) { image = img; }

/**
 * draw scene background
*/
void OwnGraphicScene::drawBackground(QPainter *painter, const QRectF &rect) {
  QGraphicsScene::drawBackground(painter, rect);
  painter->drawImage(0, 0, image);
}

/**
 * Draw a sigle, temporary rectangle.
 * @brief OwnGraphicScene::rectDraw
 * @param pt1
 * @param pt2
 *
 * Draw rect on scene
 */
void OwnGraphicScene::rectDraw(QPointF pt1, QPointF pt2) {
  itemColor.setRgb(qrand() % (255 + 1), qrand() % (255 + 1),
                   qrand() % (255 + 1));

  QPen pen;
  pen.setColor(itemColor);
  pen.setWidth(2);

  // TODO: why is selectionRect.update not working?
  if (selectionRect) {
    this->removeItem(selectionRect);
    delete selectionRect;
    selectionRect = addRect(QRectF(pt1, pt2), pen);
  } else {
    selectionRect = addRect(QRectF(pt1, pt2), pen);
  }

  this->update();
}

/**
 * mouse Press: get pressed point
 *              pause the video if is playing
 *              remove geom_item if right click
 *              activat drawmode if left click on No item
*/
void OwnGraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // video Pause, to paint item.
  emit on_btnPause_clicked();

  if (event->button() == Qt::LeftButton) isLeftPressed = true;

  if (itemAt(event->scenePos(), QTransform()) == 0) {
    if (event->button() == Qt::LeftButton) {
      // get x,y coordinate
      point1 = (event->scenePos());

      // set flag on press
      drawMode = DrawON;
    }
  } else {
    drawMode = DrawOFF;
    if (event->button() == Qt::RightButton && !isLeftPressed) {
      // TODO: I removed this, because we don't want the rect to disappear when
      // right-clicking.
    } else if (event->button() == Qt::LeftButton) {
      this->clearSelection();  // single selection mode
    }
  }

  QGraphicsScene::mousePressEvent(event);
}

/**
 * mouse move: if drawmode active, draw geom_item
*/
void OwnGraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    if (drawMode == DrawON) {
      point2 = (event->scenePos()).toPoint();
      rectDraw(point1, point2);
      this->isDrawn = true;
      isItemMove = false;
    } else {
      isItemMove = true;
    }
  }

  QGraphicsScene::mouseMoveEvent(event);
}

/**
 * mouse release: if drawmode active, set geom_item data & show popup
 *                if geom_item is moving, set geom data to update coordinate
*/
void OwnGraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (selectionRect) {
      this->removeItem(selectionRect);
      delete selectionRect;
      selectionRect = nullptr;
    }

    isLeftPressed = false;
    if (drawMode == DrawON) {
      if (isDrawn) {
        NewAnnotationDialog newAnnotationDialog(session, this->currentFrame,
                                                this->selected_obj);
        QPointF upperLeft = adjustCoordinate(point1);
        QPointF lowerRight = adjustCoordinate(point2);
        float x = std::min(upperLeft.x(), lowerRight.x());
        float y = std::min(upperLeft.y(), lowerRight.y());
        float w = std::max(upperLeft.x(), lowerRight.x()) - x;
        float h = std::max(upperLeft.y(), lowerRight.y()) - y;
        if (w <= 0 || h <= 0) return;

        newAnnotationDialog.setDimensions(x, y, w, h);
        newAnnotationDialog.move(event->scenePos().x(), event->scenePos().y());
        newAnnotationDialog.exec();

        isDrawn = false;
      }

    } else if (isItemMove) {
      // isItemMove = false;
      /// qDebug()<<"# of Rects: "<<ItemsList.size();
    }

    drawMode = DrawOFF;
  }

  QGraphicsScene::mouseReleaseEvent(event);
}

/**
 * mouse doubleClick: get geom_item data and show popup to edit geom_item.
*/
void OwnGraphicScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  if (itemAt(event->scenePos(), QTransform()) != 0) {
    QGraphicsScene::mouseDoubleClickEvent(event);
  }
}

void OwnGraphicScene::setCurrentFrame(int frame) { this->currentFrame = frame; }

void OwnGraphicScene::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

/**
 * adjustCoordinate: to calculate the correct coordinate of image/video on
 * scene.
 * @param: pt1: QPoint the coordinate on scene (mouse Position)
 *         pt2: QPoint the coordinate of image/video Frame
 *         pt3: QPoint the coordinate of scene (lower Right)(width, height)
 *
*/
QPoint OwnGraphicScene::adjustCoordinate(QPointF MousePos) {
  return QPoint(MousePos.x(), MousePos.y());
  // I commented out because there was an offset. Without it works.
  // return QPoint(((MousePos.x() * inputCoordinate.x()) /
  // this->width()),((MousePos.y() * inputCoordinate.y())  / this->height()));
}

QPointF OwnGraphicScene::resetCoordinate(QPointF RectPos) {
  return QPointF(((RectPos.x() * this->width()) / inputCoordinate.x()),
                 ((RectPos.y() * this->height()) / inputCoordinate.y()));
}

void OwnGraphicScene::on_objectSelected(AnnotatorLib::Object *obj) {
  this->selected_obj = obj;
}
