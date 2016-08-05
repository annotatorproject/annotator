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
    explicit NewObjectDialog(QWidget *parent = 0);
    ~NewObjectDialog();
    void setDimenstions(float x, float y, float w, float h);
    void setFrame(int frame);
    void setSession(AnnotatorLib::Session *session);
    void createObject();

protected:
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    int frame = 0;
    AnnotatorLib::Session *session;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewObjectDialog *ui;
};

#endif // NEWOBJECTDIALOG_H
