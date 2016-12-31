// Copyright 2016 Annotator Team
#ifndef NEWATTRIBUTEDIALOG_H
#define NEWATTRIBUTEDIALOG_H

#include <memory>

#include <AnnotatorLib/Session.h>
#include <AnnotatorLib/Object.h>

#include <QDialog>

namespace Ui {
class NewAttributeDialog;
}

class NewAttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewAttributeDialog(std::shared_ptr<AnnotatorLib::Session> session,shared_ptr<AnnotatorLib::Object> object, QWidget *parent = 0);
    ~NewAttributeDialog();

protected:
    void createAttribute();

private:
    Ui::NewAttributeDialog *ui;
    void done(int status) override;

    const std::shared_ptr<AnnotatorLib::Session> session;
    const shared_ptr<AnnotatorLib::Object> object;
};

#endif // NEWATTRIBUTEDIALOG_H
