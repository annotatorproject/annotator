#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

namespace Annotator{
namespace Plugins{
    class Haarcascade;
}
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setHaarcascade(Annotator::Plugins::Haarcascade *haarcascade);

private slots:
    void on_pushButton_clicked();

    void on_objectNameLineEdit_editingFinished();

    void on_newObjectsCheckBox_toggled(bool checked);

private:
    Ui::Widget *ui;
    Annotator::Plugins::Haarcascade *haarcascade = nullptr;

};


#endif // WIDGET_H
