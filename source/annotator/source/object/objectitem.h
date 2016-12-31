// Copyright 2016 Annotator Team
#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Session.h>
#include <QWidget>

namespace Ui {
class ObjectItem;
}

class ObjectItem : public QWidget {
  Q_OBJECT

 public:
  ObjectItem() = delete;
  explicit ObjectItem(std::shared_ptr<AnnotatorLib::Session> session,
                      shared_ptr<AnnotatorLib::Object> object,
                      QWidget *parent = 0);
  ~ObjectItem();
  void reload();
  shared_ptr<AnnotatorLib::Object> getObject();

 private slots:
  void on_windowButton_clicked();

 private:
  Ui::ObjectItem *ui;

  const shared_ptr<AnnotatorLib::Object> object;
  const std::shared_ptr<AnnotatorLib::Session> session;
};

#endif  // OBJECTITEM_H
