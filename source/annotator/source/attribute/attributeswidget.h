#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <QWidget>
#include <AnnotatorLib/Session.h>

namespace Ui {
class AttributesWidget;
}

class AttributesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AttributesWidget(QWidget *parent = 0);
    ~AttributesWidget();
    void setSession(AnnotatorLib::Session *session);
    void reload();

private:
    Ui::AttributesWidget *ui;

    AnnotatorLib::Session *session = nullptr;
};

#endif // ATTRIBUTESWIDGET_H
