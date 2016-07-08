#ifndef ANNOTATIONITEM_H
#define ANNOTATIONITEM_H

#include <AnnotatorLib/Annotation.h>
#include <QWidget>

namespace Ui {
class AnnotationItem;
}

class AnnotationItem : public QWidget {
  Q_OBJECT

 public:
  explicit AnnotationItem(AnnotatorLib::Annotation *annotation,
                          QWidget *parent = 0);
  ~AnnotationItem();
  void reload();
  AnnotatorLib::Annotation *getAnnotation();

 private:
  Ui::AnnotationItem *ui;
  AnnotatorLib::Annotation *annotation;
};

#endif  // ANNOTATIONITEM_H
