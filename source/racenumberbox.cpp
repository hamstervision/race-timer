#include "racenumberbox.h"
#include <QKeyEvent>

RaceNumberBox::RaceNumberBox(QWidget *parent)
    : QSpinBox(parent)
{

}

void RaceNumberBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Space:
        break;
    default:
        QSpinBox::keyPressEvent(event);
        return;
    }

    selectAll();
    emit valueAccepted();
}

void RaceNumberBox::focusInEvent(QFocusEvent *event)
{
    QSpinBox::focusInEvent(event);
    selectAll();
}
