#ifndef PLUGINSWIDGET_H
#define PLUGINSWIDGET_H

#include <QWidget>

namespace Ui {
class PluginsWidget;
}

class PluginsWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PluginsWidget(QWidget *parent = 0);
  ~PluginsWidget();

  void reload();

 private slots:
  void on_comboBox_currentIndexChanged(const QString &arg1);

 private:
  Ui::PluginsWidget *ui;
  QWidget *lastWidget = nullptr;
};

#endif  // PLUGINSWIDGET_H
