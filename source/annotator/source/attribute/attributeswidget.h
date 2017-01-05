// Copyright 2016-2017 Annotator Team
#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <AnnotatorLib/Session.h>
#include <QWidget>

namespace Ui {
class AttributesWidget;
}

class AttributesWidget : public QWidget {
  Q_OBJECT

 public:
  explicit AttributesWidget(QWidget *parent = 0);
  ~AttributesWidget();
  void setSession(std::shared_ptr<AnnotatorLib::Session> session);
  void reload();

 private:
  Ui::AttributesWidget *ui;

  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
};

#endif  // ATTRIBUTESWIDGET_H
