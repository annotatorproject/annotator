#ifndef RECTANGLEANNOTATION_H
#define RECTANGLEANNOTATION_H

#include <AnnotatorLib/Annotation.h>
#include "annotationgraphicsitem.h"

class RectangleAnnotation : public AnnotationGraphicsItem
{
public:
    RectangleAnnotation(AnnotatorLib::Annotation *annotation);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void init();
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);



};

#endif // RECTANGLEANNOTATION_H
