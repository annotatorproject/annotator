#include "ownslider.h"
#include <QDebug>
#include <QStyleOptionSlider>

OwnSlider::OwnSlider(QWidget *parent) : QSlider(parent) {
  // this->setMouseTracking(true);
}

/**
 *  reimplement mouse Press of QSlider
*/
void OwnSlider::mousePressEvent(QMouseEvent *event) {
  // get all information to draw QSlider
  QStyleOptionSlider opt;
  initStyleOption(&opt);
  QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt,
                                     QStyle::SC_SliderHandle, this);

  if (event->button() == Qt::LeftButton && sr.contains(event->pos()) == false) {
    if (orientation() == Qt::Vertical)
      newVal = minimum() +
               ((maximum() - minimum()) * (height() - event->y())) / height();
    else
      halfHandleWidth = (0.5 * sr.width()) + 0.5; // Correct rounding
    adaptedPosX = event->x();
    if (adaptedPosX < halfHandleWidth)
      adaptedPosX = halfHandleWidth;
    if (adaptedPosX > width() - halfHandleWidth)
      adaptedPosX = width() - halfHandleWidth;

    // get new dimensions accounting for slider handle width
    newWidth = (width() - halfHandleWidth) - halfHandleWidth;
    normalizedPosition = (adaptedPosX - halfHandleWidth) / newWidth;

    newVal = minimum() + ((maximum() - minimum()) * normalizedPosition);

    if (invertedAppearance() == true)
      setValue(maximum() - newVal);
    else {
      setValue(newVal);

      event->accept();
    }
  }

  int value = this->value();
  emit sendClickPosition(value);

  QSlider::mousePressEvent(event);
}

void OwnSlider::mouseReleaseEvent(QMouseEvent *event) {
  int value = this->value();
  emit sendClickPosition(value);
  QSlider::mouseReleaseEvent(event);
}
