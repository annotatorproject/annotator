#ifndef ATTRIBUTEITEM_H
#define ATTRIBUTEITEM_H

#include <AnnotatorLib/Attribute.h>
#include <QWidget>

namespace Ui {
class AttributeItem;
}

class AttributeItem : public QWidget {
  Q_OBJECT

 public:
  explicit AttributeItem(AnnotatorLib::Attribute *attribute,
                         QWidget *parent = 0);
  ~AttributeItem();
  void reload();
  AnnotatorLib::Attribute *getAttribute();

 private:
  Ui::AttributeItem *ui;

  AnnotatorLib::Attribute *attribute;
};

#endif  // ATTRIBUTEITEM_H
