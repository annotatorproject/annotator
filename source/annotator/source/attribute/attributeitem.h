// Copyright 2016-2017 Annotator Team
#ifndef ATTRIBUTEITEM_H
#define ATTRIBUTEITEM_H

#include <AnnotatorLib/Annotation.h>
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

  /**
   * @brief setAnnotation
   * @param annotation
   */
  void setAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation);
  shared_ptr<AnnotatorLib::Attribute> getAttribute();

 private slots:
  void on_typeComboBox_currentIndexChanged(const QString &arg1);

  void on_valueLineEdit_editingFinished();

  void on_updateButton_clicked();

 private:
  Ui::AttributeItem *ui;

  shared_ptr<AnnotatorLib::Attribute> attribute;
  shared_ptr<AnnotatorLib::Annotation> annotation;
};

#endif  // ATTRIBUTEITEM_H
