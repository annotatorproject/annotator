#ifndef REMOVEANNOTATIONDIALOG_H
#define REMOVEANNOTATIONDIALOG_H

#include <QDialog>
#include <AnnotatorLib/Session.h>
#include <AnnotatorLib/Object.h>
#include <AnnotatorLib/Commands/RemoveAnnotationRange.h>

namespace Ui {
class RemoveAnnotationDialog;
}

class RemoveAnnotationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveAnnotationDialog(std::shared_ptr<AnnotatorLib::Session> session, std::shared_ptr<AnnotatorLib::Object> object, unsigned long currentFrame, QWidget *parent = 0);
    ~RemoveAnnotationDialog();
    std::shared_ptr<AnnotatorLib::Commands::RemoveAnnotationRange> getCommand();

private:
    std::shared_ptr<AnnotatorLib::Session> session;
    std::shared_ptr<AnnotatorLib::Object> object;
    Ui::RemoveAnnotationDialog *ui;
};

#endif // REMOVEANNOTATIONDIALOG_H
