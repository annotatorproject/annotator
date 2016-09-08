#ifndef ANNOTATIONITEM_H
#define ANNOTATIONITEM_H

#include <AnnotatorLib/Annotation.h>
#include <QWidget>
#include <memory>

using std::shared_ptr;

namespace Ui {
class AnnotationItem;
}

class AnnotationItem : public QWidget {
  Q_OBJECT

 public:
  explicit AnnotationItem(shared_ptr<AnnotatorLib::Annotation> annotation,
                          QWidget *parent = 0);
  ~AnnotationItem();
  void reload();
  shared_ptr<AnnotatorLib::Annotation> getAnnotation();

 private:
  Ui::AnnotationItem *ui;
  shared_ptr<AnnotatorLib::Annotation> annotation;
};

#endif  // ANNOTATIONITEM_H
