#ifndef MOUSE_POSITION_H
#define MOUSE_POSITION_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include<QMouseEvent>

class mouse_position : public QLabel
{

    Q_OBJECT

public:
    mouse_position(QWidget *parent=0);

protected:
    //void mouseMoveEvent(QMouseEvent *mouse_event);
    void mousePressEvent(QMouseEvent *mouse_event);

signals:
    void sendMousePosition(QPoint&);

};

#endif // MOUSE_position_H
