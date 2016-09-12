#include <QMessageBox>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/RemoveAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Commands/UpdateObject.h>
#include <AnnotatorLib/Commands/RemoveObject.h>
#include <AnnotatorLib/Commands/CompressSession.h>
#include <AnnotatorLib/Commands/CompressObject.h>
#include <AnnotatorLib/Commands/CleanSession.h>
#include "commandcontroller.h"

using namespace AnnotatorLib;

static CommandController *singleton = nullptr;

CommandController::CommandController(QObject *parent) : QObject(parent) {}

CommandController *CommandController::instance() {
  if (!singleton) singleton = new CommandController();
  return singleton;
}

void CommandController::setSession(Session *session) {
  this->session = session;
}

void CommandController::execute(shared_ptr<Commands::Command> command,
                                bool request_gui_reload) {
  shared_ptr<Commands::Command> command_executed = session->execute(command);
  if (request_gui_reload && command_executed) {
    send_signals(command_executed, false);
  }
}

void CommandController::redo() {
  try {
    shared_ptr<Commands::Command> command = session->redo();
    if (command) send_signals(command, false);
  } catch (std::exception &e) {
  }
}

void CommandController::undo() {
  try {
    shared_ptr<Commands::Command> command = session->undo();
    if (command) send_signals(command, true);
  } catch (std::exception &e) {
  }


}


void CommandController::send_signals(shared_ptr<Commands::Command> command, bool undo) {

  if (std::dynamic_pointer_cast<Commands::NewAnnotation>(command)) {

      if (!undo) {
          if (std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->newObjectCreated())
            emit signal_newObject(std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation()->getObject());
          else
            emit signal_newAnnotation(std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation());
      } else {
          emit signal_removedAnnotation(std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation());
      }
      //TODO: emit signal_selectObject
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::RemoveAnnotation>(command)) {
      if (!undo) {
          emit signal_removedAnnotation(std::dynamic_pointer_cast<Commands::RemoveAnnotation>(command)->getAnnotation());
      } else {
          emit signal_newAnnotation(std::dynamic_pointer_cast<Commands::RemoveAnnotation>(command)->getAnnotation());
      }
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::RemoveObject>(command)) {
      if (!undo) {
          emit signal_removedObject(std::dynamic_pointer_cast<Commands::RemoveObject>(command)->getObject());
      } else {
          emit signal_newObject(std::dynamic_pointer_cast<Commands::RemoveObject>(command)->getObject());
      }
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::UpdateAnnotation>(command)) {
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::UpdateObject>(command)) {
      emit signal_updateObject(std::dynamic_pointer_cast<Commands::UpdateObject>(command)->getObject());
      return;
  }
  if (std::dynamic_pointer_cast<Commands::CompressObject>(command)) {
      emit signal_removedObject(std::dynamic_pointer_cast<Commands::CompressObject>(command)->getObject());
      emit signal_newObject(std::dynamic_pointer_cast<Commands::CompressObject>(command)->getObject());
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::CompressSession>(command) ||
      std::dynamic_pointer_cast<Commands::CleanSession>(command)) {
      emit signal_refreshSession();
      emit signal_requestFrameRedraw();
      return;
  }
}
