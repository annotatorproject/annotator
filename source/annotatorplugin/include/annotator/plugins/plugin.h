// Copyright 2016 Annotator Team
#ifndef PLUGIN_H
#define PLUGIN_H

#include <AnnotatorLib/AnnotatorAlgoInterface.h>
#include <AnnotatorLib/Commands/Command.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Project.h>
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

  virtual std::vector<shared_ptr<AnnotatorLib::Commands::Command>> calculate(
      shared_ptr<AnnotatorLib::Object> object,
      shared_ptr<AnnotatorLib::Frame> frame, bool execute_commands = true) {
    std::shared_ptr<AnnotatorLib::Session> session = getProject()->getSession();

    if (requiresObject()) {
      if (!object) {
        std::vector<shared_ptr<AnnotatorLib::Commands::Command>> empty;
        return empty;
      }

      setObject(object);
      shared_ptr<AnnotatorLib::Annotation> annotationAtPrevFrame =
          session->getAnnotation(std::make_shared<AnnotatorLib::Frame>(
                                     frame->getFrameNumber() - 1),
                                 object);

      if (!annotationAtPrevFrame) {
        shared_ptr<AnnotatorLib::Annotation> previousA = nullptr;
        shared_ptr<AnnotatorLib::Annotation> nextA = nullptr;
        object->findClosestKeyFrames(frame, previousA, nextA);
        annotationAtPrevFrame = previousA;
      }

      if (annotationAtPrevFrame) {
        setLastAnnotation(annotationAtPrevFrame);
      }
    }

    cv::Mat image =
        getProject()->getImageSet()->getImage(frame->getFrameNumber());
    setFrame(frame, image);

    std::vector<shared_ptr<AnnotatorLib::Commands::Command>> commands =
        getCommands();

    if (execute_commands) {
      for (shared_ptr<AnnotatorLib::Commands::Command> command : commands) {
        session->execute(command);
      }
    }
    return commands;
  }
};
}

Q_DECLARE_INTERFACE(Annotator::Plugin, "Annotator.Plugin/1.0")

#endif  // PLUGIN_H
