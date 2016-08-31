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
    explicit NewObjectDialog(AnnotatorLib::Session * session, AnnotatorLib::Object* sel_obj = nullptr, QWidget *parent = 0);
    ~NewObjectDialog();
    void setDimensions(float x, float y, float w, float h);
    void setFrame(int frame);
    void setSession(AnnotatorLib::Session *session);
    void createObject();

protected:
    bool checkValues();
    void reloadClasses();

    const AnnotatorLib::Object* selected_obj;
    Ui::NewObjectDialog *ui;
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    int frame = 0;
    AnnotatorLib::Session *session;

private slots:
    void on_radioButtonNewObj_clicked();
    void on_radioButtonSelObj_clicked();
    void done(int r) override;  //called when ok or cancel is clicked
    void on_editClassesButton_clicked();

};

#endif // NEWOBJECTDIALOG_H
