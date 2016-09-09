#include "alert.h"

void Alert::showEvent ( QShowEvent * event ) {
    currentTime = 0;
    if (autoClose) {
    this->startTimer(1000);
    }
}

void Alert::timerEvent(QTimerEvent *event)
{
    currentTime++;
    if (currentTime>=timeout) {
    this->done(0);
    }
}
