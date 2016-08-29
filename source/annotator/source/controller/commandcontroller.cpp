#include "commandcontroller.h"

#include <QMessageBox>

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

void CommandController::execute(AnnotatorLib::Commands::Command *command, bool request_gui_reload) {
  bool success = command->execute();
  if (request_gui_reload && success) emit onCommandExecute();
}

void CommandController::redo() {
  try {
    session->redo();
  } catch (std::exception &e) {
  }
  emit onCommandExecute();
}

void CommandController::undo() {
  try {
    session->undo();
  } catch (std::exception &e) {
  }
  emit onCommandUndo();
}
