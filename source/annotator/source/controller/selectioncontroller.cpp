#include "selectioncontroller.h"

using namespace AnnotatorLib;
using namespace std;

static SelectionController *singleton = nullptr;

SelectionController::SelectionController(QObject *parent) : QObject(parent) {}

SelectionController *SelectionController::instance() {
  if (!singleton) singleton = new SelectionController();
  return singleton;
}

shared_ptr<Object> SelectionController::getSelectedObject() const {
  return this->object;
}

void SelectionController::setSelectedObject(shared_ptr<Object> obj) {
  if(obj != this->object) {
    this->object = obj;
    emit signal_objectSelection(this->object);
  }
}


