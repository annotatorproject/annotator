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
    void setProject(std::shared_ptr<AnnotatorLib::Project> project);

public slots:
    void on_objectSelected(shared_ptr<AnnotatorLib::Object> object);

private:
    Ui::SelectedObject *ui;

    QPixmap getImgCrop(shared_ptr<AnnotatorLib::Annotation> annotation, int size) const;

protected:
    std::shared_ptr<AnnotatorLib::Object> object;
    std::shared_ptr<AnnotatorLib::Project> project = nullptr;

    void resizeEvent(QResizeEvent* event) override;
private slots:
    void on_commandLinkButtonLast_clicked();

private slots:
    void on_commandLinkButtonFirst_clicked();
};

#endif // SELECTEDOBJECT_H
