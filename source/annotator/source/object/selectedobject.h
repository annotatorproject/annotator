#ifndef SELECTEDOBJECT_H
#define SELECTEDOBJECT_H

#include <QWidget>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Project.h>

namespace Ui {
class SelectedObject;
}

class SelectedObject : public QWidget
{
    Q_OBJECT

public:
    explicit SelectedObject(QWidget *parent = 0);
    ~SelectedObject();
    void update(std::shared_ptr<AnnotatorLib::Object> object);
    void updateImage();
    void setProject(AnnotatorLib::Project *project);

public slots:
    void on_objectSelected(shared_ptr<AnnotatorLib::Object> object);

private:
    Ui::SelectedObject *ui;

protected:
    std::shared_ptr<AnnotatorLib::Object> object;
    AnnotatorLib::Project *project = nullptr;
};

#endif // SELECTEDOBJECT_H
