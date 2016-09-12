#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include <memory>
#include <QObject>
#include <AnnotatorLib/Object.h>

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

signals:
  void signal_objectSelection(shared_ptr<AnnotatorLib::Object> object);

 protected:
  shared_ptr<AnnotatorLib::Object> object;
};

#endif  // SELECTIONCONTROLLER_H
