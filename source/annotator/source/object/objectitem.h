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
  ObjectItem() = delete;
  explicit ObjectItem(shared_ptr<AnnotatorLib::Object> object,
                      QWidget *parent = 0);
  ~ObjectItem();
  void reload();
  shared_ptr<AnnotatorLib::Object> getObject();

 private:
  Ui::ObjectItem *ui;

  const shared_ptr<AnnotatorLib::Object> object;
};

#endif  // OBJECTITEM_H
