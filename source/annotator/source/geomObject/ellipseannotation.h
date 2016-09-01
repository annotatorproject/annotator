#ifndef ELLIPSEANNOTATION_H
#define ELLIPSEANNOTATION_H

#include <AnnotatorLib/Annotation.h>
#include <QPen>
#include "annotationgraphicsitem.h"

class EllipseAnnotation : public AnnotationGraphicsItem {
 public:
  EllipseAnnotation(AnnotatorLib::Annotation *annotation);

  virtual QRectF boundingRect() const;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget = 0);

 protected:
  void init();
};

#endif  // ELLIPSEANNOTATION_H
