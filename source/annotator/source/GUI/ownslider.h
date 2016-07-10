#ifndef OWNSLIDER_H
#define OWNSLIDER_H

#include <QSlider>
#include <QMouseEvent>



class OwnSlider : public QSlider
{
    Q_OBJECT


public:
    explicit OwnSlider(QWidget  *parent = 0);

signals:
    void sendClickPosition(int);

public slots:

protected:
    void mousePressEvent ( QMouseEvent * event );

private:
    double halfHandleWidth;
    int newVal;
    int adaptedPosX;
    double newWidth;
    double normalizedPosition;
};

#endif // OWNSLIDER_H
