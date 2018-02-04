#include "mouse_position.h"

mouse_position::mouse_position(QWidget *parent): QLabel (parent)
{ 

}


void mouse_position::mousePressEvent(QMouseEvent *mouse_event)
{
    QPoint pos = mouse_event->pos();
    if(mouse_event->button()== Qt::LeftButton || mouse_event->button()== Qt::RightButton)
    {
        emit sendMousePosition(pos);
    }
}
