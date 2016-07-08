#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QDialog>
#include "interpolation/popup.h"
#include <QStandardItemModel>

namespace Ui {
class NewWidget;
}

class NewWidget : public QDialog
{
    Q_OBJECT

public:
    explicit NewWidget(QWidget *parent = 0);
    ~NewWidget();

private slots:
    void on_btnPopSave_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_btnClose_clicked();

private:
    Ui::NewWidget *ui;
    Popup pop;

signals:
    void setNewWidget(const QString &labelText, const QString &typeText);

};

#endif // NEWWIDGET_H
