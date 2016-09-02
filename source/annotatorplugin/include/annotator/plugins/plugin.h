#ifndef PLUGIN_H
#define PLUGIN_H

#include <AnnotatorLib/AnnotatorAlgoInterface.h>
#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtWidgets/QWidget>

namespace Annotator {

class Plugin : virtual public QObject,
               public AnnotatorAlgo::AnnotatorAlgoInterface {
 public:
  virtual QString getName() = 0;
  virtual QWidget* getWidget() = 0;
};
}

Q_DECLARE_INTERFACE(Annotator::Plugin, "Annotator.Plugin/1.0")

#endif  // PLUGIN_H
