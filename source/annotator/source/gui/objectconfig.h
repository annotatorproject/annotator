#ifndef OBJECTCONFIG_H
#define OBJECTCONFIG_H

#include <QDialog>

namespace Ui {
class ObjectConfig;
}

class ObjectConfig : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectConfig(QWidget *parent = 0);
    ~ObjectConfig();

private:
    Ui::ObjectConfig *ui;
};

#endif // OBJECTCONFIG_H
