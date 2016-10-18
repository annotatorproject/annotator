#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include <AnnotatorLib/Object.h>
#include <QObject>
#include <memory>

/**
 * @brief The SelectionController class
 */
class SelectionController : public QObject {
  Q_OBJECT
 public:
  explicit SelectionController(QObject *parent = 0);
  static SelectionController *instance();

  void setSelectedObject(shared_ptr<AnnotatorLib::Object> obj);
  shared_ptr<AnnotatorLib::Object> getSelectedObject() const;

  void setSelectedFrame(shared_ptr<AnnotatorLib::Frame> f);
  void setSelectedFrame(long nmb);

 signals:
  void signal_objectSelection(shared_ptr<AnnotatorLib::Object> object);
  void signal_frameSelection(long nmb);

 protected:
  shared_ptr<AnnotatorLib::Object> object;
};

#endif  // SELECTIONCONTROLLER_H
