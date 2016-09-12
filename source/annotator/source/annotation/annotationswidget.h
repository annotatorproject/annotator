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
  void signal_objectSelection(shared_ptr<AnnotatorLib::Object> object);
  void signal_frameSelection(long frame);

 private slots:
  void on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                        QTreeWidgetItem *previous);

  void on_refreshButton_clicked();
  //TODO: slots for added or removed objects/annotations (we do not have to build the hole tree every time...)

 private:
  Ui::AnnotationsWidget *ui;

  AnnotatorLib::Session *session = nullptr;
};

#endif  // ANNOTATIONSWIDGET_H
