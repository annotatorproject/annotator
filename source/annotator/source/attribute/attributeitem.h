#ifndef ATTRIBUTEITEM_H
#define ATTRIBUTEITEM_H

#include <AnnotatorLib/Attribute.h>
#include <QWidget>
#include <memory>

using std::shared_ptr;

namespace Ui {
class AttributeItem;
}

class AttributeItem : public QWidget {
  Q_OBJECT

 public:
  explicit AttributeItem(shared_ptr<AnnotatorLib::Attribute> attribute,
                         QWidget *parent = 0);
  ~AttributeItem();
  void reload();

  shared_ptr<AnnotatorLib::Attribute> getAttribute();

 private:
  Ui::AttributeItem *ui;

  shared_ptr<AnnotatorLib::Attribute> attribute;
};

#endif  // ATTRIBUTEITEM_H
