#ifndef OBJECTSWIDGET_H
#define OBJECTSWIDGET_H

#include <QWidget>
#include <AnnotatorLib/Session.h>
#include <AnnotatorLib/Object.h>

namespace Ui {
class ObjectsWidget;
}

class ObjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectsWidget(QWidget *parent = 0);
    ~ObjectsWidget();
    void setSession(AnnotatorLib::Session *session);
    void reload();

signals:
    void objectSelected(AnnotatorLib::Object *object);

private slots:
    void selectObject(AnnotatorLib::Object *object);
    void on_listWidget_itemSelectionChanged();

private:
    Ui::ObjectsWidget *ui;

    AnnotatorLib::Session *session = nullptr;
};

#endif // OBJECTSWIDGET_H
