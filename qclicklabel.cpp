#include "qclicklabel.h"

QClickLabel::QClickLabel(QWidget* parent, Qt::WindowFlags)
    : QLabel(parent) {
}

QClickLabel::~QClickLabel() {}

void QClickLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit clickedRight();
    }
    else
    {
        emit clicked();
    }
}
