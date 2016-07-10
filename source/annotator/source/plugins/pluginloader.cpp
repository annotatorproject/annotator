#include "pluginloader.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

using namespace Annotator;

PluginLoader* PluginLoader::instance = nullptr;

PluginLoader::PluginLoader(QObject *parent) :
    QObject(parent)
{
    loadPlugins();
}

PluginLoader::~PluginLoader(){
}

void PluginLoader::destroyInstance(){
    if (instance)
        delete instance;
    instance = nullptr;
}

PluginLoader& PluginLoader::getInstance() {
    if (!instance)
        instance = new PluginLoader(0);
    return *instance;
}

Plugin *PluginLoader::getPlugin(QString name){
    for(Plugin* plugin: this->plugins){
        if(plugin->getName() == name)
            return plugin;
    }
    return nullptr;
}

void PluginLoader::setCurrent(QString name)
{
    Plugin* plugin = getPlugin(name);
    if(plugin)
        this->current = plugin;
}

Plugin *PluginLoader::getCurrent()
{
    return current;
}

void PluginLoader::loadPlugins() {
    // Loads Plugins in plugins dir
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    loadPlugins(pluginsDir.currentPath());

    for(QString path: QCoreApplication::libraryPaths())
    {
        loadPlugins(path);
        loadPlugins(path + QCoreApplication::applicationName().toLower());
    }
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug"
            || pluginsDir.dirName().toLower() == "release") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#elif defined(Q_OS_LINUX)
    loadPlugins("/usr/lib/" + QCoreApplication::applicationName().toLower());
    loadPlugins("/usr/lib64/" + QCoreApplication::applicationName().toLower());
#endif
    loadPlugins(pluginsDir.currentPath());
}

void PluginLoader::loadPlugins(QString dir) {
    QDir pluginsDir(dir);
    if (pluginsDir.exists(dir)) {
        if (!pluginsDir.cd("plugins"))
            return;
    }

    for(QString fileName: pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

        if (plugin) {
            // loads Plugins
            Plugin *plugin_ = qobject_cast<
                    Plugin *> (plugin);
            if (plugin_) {
                addPlugin(plugin_);
            } else {
                delete plugin_;
            }
        }else{
        }
    }
}

void PluginLoader::addPlugin(Plugin *plugin) {
    if (plugin) {
        for(Plugin * plugin_: plugins)
        {
            if (plugin_->getName() == plugin->getName())
                return;
        }
        plugins.append(plugin);
    }
}

QList<Plugin *> PluginLoader::getPlugins(){
    return this->plugins;
}
