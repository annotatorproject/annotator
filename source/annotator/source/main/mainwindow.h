#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include "player.h"
#include <AnnotatorLib/Project.h>
#include "annotation/annotationswidget.h"
#include "object/objectswidget.h"
#include "attribute/attributeswidget.h"
#include "plugins/pluginswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectSignalSlots();

    void setWindowTitle(AnnotatorLib::Project * project);


    void setRateValue(QString value);

    void openProject(AnnotatorLib::Project *project);

protected:
    /**
     * @brief loadRecentProjects
     * Loads recent projects files from settings and creates menu items for them.
     */
    void loadRecentProjects();

    /**
     * @brief addRecentProject
     * @param projectfile The project file to save in settings.
     *
     */
    void addRecentProject(QString projectfile);

public slots:
    void reloadWidgets();

    void closeEvent (QCloseEvent *event);

private slots:
    void on_actionOpen_Project_triggered();

    void on_actionClose_Project_triggered();

    void on_actionNew_Project_triggered();

    void on_actionQuit_triggered();

    void on_actionSave_Project_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionAuto_Annotate_triggered();

    void on_actionAuto_Annotate_toggled(bool arg1);

    void on_actionAbout_triggered();

    void on_actionClasses_triggered();

protected:
    Player playerWidget;
    AnnotationsWidget annotationsWidget;
    ObjectsWidget objectsWidget;
    AttributesWidget attributesWidget;
    PluginsWidget pluginsWidget;

    AnnotatorLib::Project * project = nullptr;
    AnnotatorLib::Session * session = nullptr;
    QLabel * rateLabel;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
