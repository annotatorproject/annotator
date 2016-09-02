#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "plugin.h"

#include <QtCore/QList>
#include <QtCore/QObject>

namespace Annotator {

class PluginLoader : public QObject {
  Q_OBJECT
 public:
  ~PluginLoader();

  static void destroyInstance();
  static PluginLoader &getInstance();

  Plugin *getPlugin(QString name);
  void setCurrent(QString name);
  Plugin *getCurrent();

  // InputPlugin
  QList<Plugin *> getPlugins();

 signals:

 public slots:

 private:
  explicit PluginLoader(QObject *parent = 0);
  static PluginLoader *instance;
  void loadPlugins();
  QList<Plugin *> plugins;
  void loadPlugins(QString dir);
  void addPlugin(Plugin *plugin);
  Plugin *current = nullptr;
};
}

#endif  // PLUGINLOADER_H
