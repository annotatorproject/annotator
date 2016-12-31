#ifndef NEWATTRIBUTEDIALOG_H
#define NEWATTRIBUTEDIALOG_H

#include <QDialog>

namespace Ui {
class NewAttributeDialog;
}

class NewAttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewAttributeDialog(QWidget *parent = 0);
    ~NewAttributeDialog();

private:
    Ui::NewAttributeDialog *ui;
};

#endif // NEWATTRIBUTEDIALOG_H
