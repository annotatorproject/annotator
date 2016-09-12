#ifndef ANNOTATIONSWIDGET_H
#define ANNOTATIONSWIDGET_H

#include <unordered_map>
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
  void initWidget();
  void resizeEvent(QResizeEvent *) override;

 signals:
  void signal_frameSelection(long frame);

 public slots:
  void on_refreshSession();
  void on_objectAdded(shared_ptr<AnnotatorLib::Object> object) { addObject(object); }
  void on_objectRemoved(shared_ptr<AnnotatorLib::Object> object);
  void on_annotationAdded(shared_ptr<AnnotatorLib::Annotation> a);
  void on_annotationRemoved(shared_ptr<AnnotatorLib::Annotation> a);
  void on_objectSelected(shared_ptr<AnnotatorLib::Object> object);

 private slots:
  void on_treeWidget_currentItemChanged(QTreeWidgetItem *current,
                                        QTreeWidgetItem *previous);

 protected:
  Ui::AnnotationsWidget *ui;
  std::unordered_map<unsigned long, int> objectIndexMap;
  AnnotatorLib::Session *session = nullptr;

  void refreshHoleSession();
  void addAnnotation(shared_ptr<AnnotatorLib::Annotation> annotation,
                     QTreeWidgetItem *item);
  void addObject(shared_ptr<AnnotatorLib::Object> object);
};

#endif  // ANNOTATIONSWIDGET_H
