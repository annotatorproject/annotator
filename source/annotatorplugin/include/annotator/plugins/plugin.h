#ifndef PLUGIN_H
#define PLUGIN_H

#include <AnnotatorLib/AnnotatorAlgoInterface.h>
#include <AnnotatorLib/Commands/Command.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Session.h>
#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include <QtWidgets/QWidget>

namespace Annotator {

class Plugin : virtual public QObject,
               public AnnotatorAlgo::AnnotatorAlgoInterface {
public:
  virtual QString getName() = 0;
  virtual QWidget *getWidget() = 0;
  virtual std::vector<shared_ptr<AnnotatorLib::Commands::Command>>
  calculate(shared_ptr<AnnotatorLib::Object> object,
            shared_ptr<AnnotatorLib::Frame> frame, cv::Mat image) {
    setObject(object);

    setFrame(frame, image);
    setLastAnnotation(object->getLastAnnotation());
    std::vector<shared_ptr<AnnotatorLib::Commands::Command>> commands =
        getCommands();
    for (shared_ptr<AnnotatorLib::Commands::Command> command : commands) {
      getSession()->execute(command);
    }
    return commands;
  }
};
}

Q_DECLARE_INTERFACE(Annotator::Plugin, "Annotator.Plugin/1.0")

#endif // PLUGIN_H
