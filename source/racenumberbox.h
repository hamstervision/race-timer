#ifndef RACENUMBERBOX_H
#define RACENUMBERBOX_H

#include <QSpinBox>

class QKeyEvent;
class QFocusEvent;

class RaceNumberBox : public QSpinBox
{
    Q_OBJECT

public:
    RaceNumberBox(QWidget *parent = nullptr);

signals:
    void valueAccepted();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
};

#endif // RACENUMBERBOX_H
