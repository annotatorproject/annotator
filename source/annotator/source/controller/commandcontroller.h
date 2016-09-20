#ifndef COMMANDCONTROLLER_H
#define COMMANDCONTROLLER_H

#include <AnnotatorLib/Commands/Command.h>
#include <AnnotatorLib/Session.h>
#include <QObject>

/**
 * @brief The CommandController class
 */
class CommandController : public QObject {
  Q_OBJECT
 public:
  explicit CommandController(QObject *parent = 0);
  static CommandController *instance();

  void setSession(shared_ptr<AnnotatorLib::Session> session);
  void setParentWidget(QWidget *parent);

  void doEmitRefreshSession();

 signals:
  void signal_newObject(shared_ptr<AnnotatorLib::Object>);
  void signal_newAnnotation(shared_ptr<AnnotatorLib::Annotation>);
  void signal_updateAnnotation(shared_ptr<AnnotatorLib::Annotation>);
  void signal_updateObject(shared_ptr<AnnotatorLib::Object>);
  void signal_removedObject(shared_ptr<AnnotatorLib::Object>);
  void signal_removedAnnotation(shared_ptr<AnnotatorLib::Annotation>);
  void signal_requestFrameRedraw();
  void signal_refreshSession();

 public slots:
  void execute(shared_ptr<AnnotatorLib::Commands::Command> command,
               bool request_gui_reload = true, bool show_busy_wheel = false);
  void redo();
  void undo();

 protected:
  std::shared_ptr<AnnotatorLib::Session> session = nullptr;
  QWidget *main_widget = nullptr;

  void send_signals(shared_ptr<AnnotatorLib::Commands::Command> command, bool undo);
  void enableBusySpinner(bool enable);
};

#endif  // COMMANDCONTROLLER_H
