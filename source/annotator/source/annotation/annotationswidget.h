#ifndef ANNOTATIONSWIDGET_H
#define ANNOTATIONSWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <AnnotatorLib/Session.h>

namespace Ui {
class AnnotationsWidget;
}

class AnnotationsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnnotationsWidget(QWidget *parent = 0);
    ~AnnotationsWidget();

    void setSession(AnnotatorLib::Session *session);
    void reload();
    void addAnnotation(AnnotatorLib::Annotation * annotation, QTreeWidgetItem * item);
    void addObject(AnnotatorLib::Object * object);


signals:
    void objectSelected(AnnotatorLib::Object *object);
    void frameSelected(long frame);

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_refreshButton_clicked();

private:
    Ui::AnnotationsWidget *ui;

    AnnotatorLib::Session *session = nullptr;
};

#endif // ANNOTATIONSWIDGET_H
