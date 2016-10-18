#ifndef PLUGINSWIDGET_H
#define PLUGINSWIDGET_H

#include <AnnotatorLib/Object.h>
#include <QWidget>
#include <memory>

using std::shared_ptr;

namespace Ui {
class PluginsWidget;
}

class PluginsWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PluginsWidget(QWidget *parent = 0);
  ~PluginsWidget();

  void reload();
  void setAutoAnnotate(bool b) { autoAnnotate = b; }

 signals:
  void signal_autoAnnotate(bool);

 public slots:
  void on_objectSelected(shared_ptr<AnnotatorLib::Object>);

 private slots:
  void on_auto_annotate_checkBox_clicked(bool checked);
  void on_comboBox_currentIndexChanged(const QString &arg1);

 private:
  bool autoAnnotate = false;
  Ui::PluginsWidget *ui;
  QWidget *lastWidget = nullptr;
};

#endif  // PLUGINSWIDGET_H
