#include "pluginswidget.h"
#include "controller/commandcontroller.h"
#include "plugins/pluginloader.h"
#include "ui_pluginswidget.h"

#include <annotator/plugins/plugin.h>

PluginsWidget::PluginsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PluginsWidget) {
  ui->setupUi(this);

  reload();
}

PluginsWidget::~PluginsWidget() {
  ui->comboBox->clear();
  if (lastWidget != nullptr) {
    ui->layout->removeWidget(lastWidget);
    lastWidget->setParent(nullptr);
  }
  delete ui;
}

void PluginsWidget::reload() {
  ui->comboBox->clear();
  for (Annotator::Plugin *plugin :
       Annotator::PluginLoader::getInstance().getPlugins().toStdList()) {
    ui->comboBox->addItem(plugin->getName());
  }
  on_comboBox_currentIndexChanged(ui->comboBox->currentText());
  this->updateGeometry();
  ui->gridLayout->update();
  ui->scrollArea->updateGeometry();
}

void PluginsWidget::on_objectSelected(shared_ptr<AnnotatorLib::Object> obj) {
  if (autoAnnotate) ui->comboBox->setDisabled(!obj);
  Annotator::PluginLoader::getInstance().getCurrent()->setObject(obj);
}

void PluginsWidget::on_comboBox_currentIndexChanged(const QString &arg1) {
  Annotator::PluginLoader::getInstance().setCurrent(arg1);
  Annotator::Plugin *plugin =
      Annotator::PluginLoader::getInstance().getPlugin(arg1);

  if (plugin) {
    if (lastWidget != nullptr) {
      ui->layout->removeWidget(lastWidget);
      // ui->layout->takeAt(0);
      lastWidget->setParent(nullptr);
    }
    lastWidget = plugin->getWidget();
    ui->layout->addWidget(lastWidget);
  }
}

void PluginsWidget::on_auto_annotate_checkBox_clicked(bool checked) {
  this->autoAnnotate = checked;
  this->ui->comboBox->setEnabled(checked);
  if (this->lastWidget) lastWidget->setEnabled(checked);
  emit signal_autoAnnotate(checked);
}

void PluginsWidget::on_auto_annotate_checkBox_toggled(bool checked) {
  CommandController::instance()->doEmitRefreshSession();
}
