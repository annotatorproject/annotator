// Copyright 2016-2017 Annotator Team
#ifndef ANNOTATIONGRAPHICSITEM_H
#define ANNOTATIONGRAPHICSITEM_H

#include <QBrush>
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPen>
#include <memory>
#include <mutex>

#include "corner.h"
#include "player.h"

using std::weak_ptr;

class QAction;
class Annotation;
class Object;

class AnnotationGraphicsItem : public QObject, public QGraphicsItem {
  Q_OBJECT

 public:
  AnnotationGraphicsItem() = delete;
  AnnotationGraphicsItem(shared_ptr<AnnotatorLib::Annotation> annotation);
  virtual ~AnnotationGraphicsItem();
  shared_ptr<AnnotatorLib::Annotation> getAnnotation() const;

  void setPlayer(Player *player);

  bool isAnnotationSelected() const;

  void highlight(const int reason = -1);
  void hideHighlight();

 protected:
  QPen pen;
  QBrush brush;
  QColor borderColor;
  QColor originColor;
  QGraphicsTextItem idText;
  QGraphicsPixmapItem imWarning;

  Player *player;

  qreal rectX;
  qreal rectY;
  qreal width;
  qreal height;
  Corner *corners[4];

  QBrush getGradient();

  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

 private:
  shared_ptr<AnnotatorLib::Annotation> annotation;

  QAction *action_del;
  QAction *action_del_following;
  QAction *action_del_range;
  QAction *action_edit;
  QAction *action_edit_obj;
  QAction *action_del_obj;
  QAction *action_compress_obj;
  QAction *action_goto_first;
  QAction *action_goto_last;

  void setSelected(bool b);
  void setCornerPositions();
  void getCornerPositions(Corner *corner, qreal x, qreal y);
  void changeAnnotationPosition(int x, int y);
  void changeAnnotationSize(int x, int y, int w, int h);
  void initCorners();
  void initIdText();
  void initializeLowConfidenceWarningSign();
  void initActions();
  QColor idToColor(long id) const;

 public slots:
  void on_objectSelected(shared_ptr<AnnotatorLib::Object>);

 private slots:
  void removeAnnotation();
  void removeFollowingAnnotations();
  void removeAnnotationRange();
  void removeObject();
  /**
   * @brief editAnnotation
   * show window for annotation
   */
  void editAnnotation();
  /**
   * @brief editObject
   * show window for annotation's object
   */
  void editObject();
  void compressObject();
  void goToLast();
  void goToFirst();
};

#endif  // ANNOTATIONGRAPHICSITEM_H
