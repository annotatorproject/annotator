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

  void setSession(AnnotatorLib::Session *session);

 signals:
  void onCommandExecute();
  void onCommandUndo();

 public slots:
  void execute(shared_ptr<AnnotatorLib::Commands::Command> command,
               bool request_gui_reload = true);
  void redo();
  void undo();

 protected:
  AnnotatorLib::Session *session = nullptr;
};

#endif  // COMMANDCONTROLLER_H
