#ifndef PLUGINRUNNER_H
#define PLUGINRUNNER_H

#include "annotator/plugins/plugin.h"

#include <QDialog>
#include <AnnotatorLib/Project.h>
#include <AnnotatorLib/Object.h>

namespace Ui {
class PluginRunner;
}

class PluginRunner : public QDialog
{
    Q_OBJECT

public:
    explicit PluginRunner(std::shared_ptr<AnnotatorLib::Project> project, QWidget *parent = 0);
    ~PluginRunner();
    QPixmap getImgCrop(long frame, int size);

private slots:

    void on_startFrameSpinBox_valueChanged(int arg1);

    void on_endFrameSpinBox_valueChanged(int arg1);

    void on_startButton_clicked();

    void on_selectAllObjectsButton_clicked();

    void on_unselectAllObjectsButton_clicked();

private:
    bool stopCalculation = true;
    Ui::PluginRunner *ui;
    std::shared_ptr<AnnotatorLib::Project> project;

    void initPluginsList();
    void initObjectsList();

    void addObject(shared_ptr<AnnotatorLib::Object> object);
    void calculate(shared_ptr<AnnotatorLib::Object> object, Annotator::Plugin *plugin, int start, int end);

    std::unordered_map<unsigned long, int> objectIdToRowMap;
    std::unordered_map<int, unsigned long> objectRowToIdMap;


};

#endif // PLUGINRUNNER_H
