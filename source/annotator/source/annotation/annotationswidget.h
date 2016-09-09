#ifndef ANNOTATIONSWIDGET_H
#define ANNOTATIONSWIDGET_H

#include <AnnotatorLib/Session.h>
#include <QTreeWidgetItem>
#include <QWidget>

namespace Ui {
class AnnotationsWidget;
}

class AnnotationsWidget : public QWidget {
  Q_OBJECT

 public:
  explicit AnnotationsWidget(QWidget *parent = 0);
  ~AnnotationsWidget();

  void setSession(AnnotatorLib::Session *session);
  void reload();
  void resizeEvent(QResizeEvent *) override;
  void addAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation,
                     QTreeWidgetItem *item);
  void addObject(shared_ptr<AnnotatorLib::Object> object);

 signals:
  void objectSelected(shared_ptr<AnnotatorLib::Object> object);
  void frameSelected(long frame);

 private slots:
  void on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                        QTreeWidgetItem *previous);

  void on_refreshButton_clicked();

 private:
  Ui::AnnotationsWidget *ui;

  AnnotatorLib::Session *session = nullptr;
};

#endif  // ANNOTATIONSWIDGET_H
