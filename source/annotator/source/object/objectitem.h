#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <AnnotatorLib/Object.h>
#include <QWidget>

namespace Ui {
class ObjectItem;
}

class ObjectItem : public QWidget {
  Q_OBJECT

 public:
  explicit ObjectItem(AnnotatorLib::Object *object, QWidget *parent = 0);
  ~ObjectItem();
  void reload();
  AnnotatorLib::Object *getObject();

 private:
  Ui::ObjectItem *ui;

  AnnotatorLib::Object *object;
};

#endif  // OBJECTITEM_H
