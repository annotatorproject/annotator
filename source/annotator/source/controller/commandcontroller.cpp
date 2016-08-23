#include "commandcontroller.h"

static CommandController *singleton = nullptr;

CommandController::CommandController(QObject *parent) : QObject(parent) {}

CommandController *CommandController::instance() {
  if (!singleton)
    singleton = new CommandController();
  return singleton;
}

void CommandController::setSession(AnnotatorLib::Session *session) {
  this->session = session;
}

void CommandController::execute(AnnotatorLib::Commands::Command *command) {
  command->execute();
  emit onExecute();
}

void CommandController::redo() {
  try {
    session->redo();
  } catch (std::exception &e) {
  }
  emit onExecute();
}

void CommandController::undo() {
  try {
    session->undo();
  } catch (std::exception &e) {
  }
  emit onUndo();
}
