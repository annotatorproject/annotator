#ifndef ANNOTATIONGRAPHICSITEM_H
#define ANNOTATIONGRAPHICSITEM_H

#include <memory>
#include <QGraphicsItem>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsTextItem>

#include "player.h"
#include "corner.h"

using std::weak_ptr;

class QAction;
class Annotation;
class Object;

class AnnotationGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    static AnnotationGraphicsItem* selected_annotation_item;

    AnnotationGraphicsItem() = delete;
    AnnotationGraphicsItem(shared_ptr<AnnotatorLib::Annotation> annotation);
    virtual ~AnnotationGraphicsItem();
    shared_ptr<AnnotatorLib::Annotation> getAnnotation() const;

    static void setSelectedAnnotation(shared_ptr<AnnotatorLib::Annotation>);
    static shared_ptr<AnnotatorLib::Annotation> getSelectedAnnotation();

    void setPlayer(Player *player);

    bool isAnnotationSelected() const;

    void highlight(const int reason = -1);
    void hideHighlight();

signals:
    void objectSelected(shared_ptr<AnnotatorLib::Object> object);

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
    Corner* corners[4];

    QBrush getGradient();

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    static weak_ptr<AnnotatorLib::Annotation> selected_annotation;

    shared_ptr<AnnotatorLib::Annotation> annotation;

    QAction* action_del;
    QAction* action_edit;
    QAction* action_del_obj;
    QAction* action_compress_obj;
    QAction* action_goto_first;
    QAction* action_goto_last;


    void setCornerPositions();
    void getCornerPositions(Corner *corner, qreal x, qreal y);
    void changeAnnotationPosition(int x, int y);
    void changeAnnotationSize(int x, int y, int w, int h);
    void initCorners();
    void initIdText();
    QColor idToColor(long id) const;

private slots:
    void removeAnnotation();
    void removeObject();
    void editAnnotation();
    void compressObject();
    void goToLast();
    void goToFirst();

};

#endif // ANNOTATIONGRAPHICSITEM_H
