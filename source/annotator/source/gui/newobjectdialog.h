#ifndef NEWOBJECTDIALOG_H
#define NEWOBJECTDIALOG_H

#include <QDialog>
#include <AnnotatorLib/Session.h>


namespace Ui {
class NewObjectDialog;
}

class NewObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewObjectDialog(AnnotatorLib::Session * session, unsigned long frame_nmb, AnnotatorLib::Object* sel_obj = nullptr, QWidget *parent = 0);
    ~NewObjectDialog();
    void setDimensions(float x, float y, float w, float h);
    void createObject();

public slots:
    void on_radioButtonSelObj_clicked();
    void on_radioButtonNewObj_clicked();
protected:
    bool checkValues();
    void reloadClasses();

    const AnnotatorLib::Object* selected_obj;
    const unsigned long frame_nmb = 0;
    AnnotatorLib::Session *session;

    Ui::NewObjectDialog *ui;
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

private slots:
    void on_radioButtonNewObj_clicked();
    void on_radioButtonSelObj_clicked();
    void done(int r) override;  //called when ok or cancel is clicked
    void on_editClassesButton_clicked();

};

#endif // NEWOBJECTDIALOG_H
