#include <QMessageBox>
//#include <QProgressBar>
//#include <QMovie>
//#include <QLabel>
#include <AnnotatorLib/Commands/NewAnnotation.h>
#include <AnnotatorLib/Commands/RemoveAnnotation.h>
#include <AnnotatorLib/Commands/UpdateAnnotation.h>
#include <AnnotatorLib/Commands/UpdateObject.h>
#include <AnnotatorLib/Commands/RemoveObject.h>
#include <AnnotatorLib/Commands/CompressSession.h>
#include <AnnotatorLib/Commands/CompressObject.h>
#include <AnnotatorLib/Commands/CleanSession.h>
#include "gui/qtwaitingspinner/waitingspinnerwidget.h"
#include "selectioncontroller.h"
#include "commandcontroller.h"

using namespace AnnotatorLib;

static CommandController *singleton = nullptr;

CommandController::CommandController(QObject *parent) : QObject(parent) {}

CommandController *CommandController::instance() {
  if (!singleton) singleton = new CommandController();
  return singleton;
}

void CommandController::setSession(std::shared_ptr<Session> session) {
  this->session = session;
}

void CommandController::setParentWidget(QWidget *parent)
{
    main_widget = parent;
}

void CommandController::doEmitRefreshSession()
{
    emit signal_refreshSession();
}

void CommandController::execute(shared_ptr<Commands::Command> command,
                                bool request_gui_reload,
                                bool show_busy_wheel) {


  if (show_busy_wheel) enableBusySpinner(true);
  shared_ptr<Commands::Command> command_executed = session->execute(command);
  if (show_busy_wheel) enableBusySpinner(false);

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
          if (std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->newObjectCreated()) {
            shared_ptr<Object> new_obj = std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation()->getObject();
            emit signal_newObject(new_obj);
            SelectionController::instance()->setSelectedObject(new_obj);
          } else
            emit signal_newAnnotation(std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation());
      } else {
          emit signal_removedAnnotation(std::dynamic_pointer_cast<Commands::NewAnnotation>(command)->getAnnotation());
      }
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::RemoveAnnotation>(command)) {
      shared_ptr<Annotation> a = std::dynamic_pointer_cast<Commands::RemoveAnnotation>(command)->getAnnotation();
      if (!undo) {          
          emit signal_removedAnnotation(a);
          if (a->getObject() == SelectionController::instance()->getSelectedObject() && a->getObject()->getAnnotations().size() == 0)
            SelectionController::instance()->setSelectedObject(nullptr);
      } else {
          emit signal_newAnnotation(a);
      }
      emit signal_requestFrameRedraw();
      return;
  }
  if (std::dynamic_pointer_cast<Commands::RemoveObject>(command)) {
      if (!undo) {
          emit signal_removedObject(std::dynamic_pointer_cast<Commands::RemoveObject>(command)->getObject());
          if (SelectionController::instance()->getSelectedObject() ==
              std::dynamic_pointer_cast<Commands::RemoveObject>(command)->getObject()) {
                SelectionController::instance()->setSelectedObject(nullptr);
          }
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
      SelectionController::instance()->setSelectedObject(nullptr);
      return;
    }
}

void CommandController::enableBusySpinner(bool enable) {
  static WaitingSpinnerWidget* spinner;
  if (enable) {
    if (spinner == nullptr) {
      if (main_widget != nullptr)
        spinner = new WaitingSpinnerWidget(Qt::ApplicationModal, main_widget, true, true);
      else
        spinner = new WaitingSpinnerWidget(Qt::ApplicationModal);

      spinner->setRoundness(70.0);
      spinner->setMinimumTrailOpacity(25.0);
      spinner->setTrailFadePercentage(70.0);
      spinner->setNumberOfLines(12);
      spinner->setLineLength(11);
      spinner->setLineWidth(6);
      spinner->setInnerRadius(11);
      spinner->setRevolutionsPerSecond(1);
      spinner->setColor(QColor(81, 81, 81));
    }
    spinner->start(); // starts spinning
  } else {
    spinner->stop();
  }
}

