// Copyright 2016-2017 Annotator Team
#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <AnnotatorLib/Session.h>
#include <AnnotatorLib/Frame.h>
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

  void setFrame(std::shared_ptr<AnnotatorLib::Frame> frame);
  void reload();

public slots:
    void setFrame(long frame_number);

private slots:
  void on_removeAttributeButton_clicked();

  void on_addAttributeButton_clicked();

private:
  Ui::AttributesWidget *ui;

  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
  std::shared_ptr<AnnotatorLib::Frame> frame = nullptr;
};

#endif  // ATTRIBUTESWIDGET_H
