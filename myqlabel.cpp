#include "myqlabel.h"

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{

}

void MyQLabel::mousePressEvent(QMouseEvent *mouseEvent)
{
    if (mouseEvent->button()==Qt::LeftButton)
        // If the left button of mouse is pressed,...
    {
        mousePressedLeft=true;
        mouseReleasedLeft=false;
        mousePressedRight=false;
        mouseReleasedRight=false;
        mousePressedPointLeft=mouseEvent->pos();
    }
    else if(mouseEvent->button()==Qt::RightButton)
        // If the right button of mouse is pressed,...
    {
        mousePressedLeft=false;
        mouseReleasedLeft=false;
        mousePressedRight=true;
        mouseReleasedRight=false;
        mousePressedPointRight=mouseEvent->pos();
    }

    emit signalMouseClicked(mouseEvent);
}

void MyQLabel::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    mousePressedLeft=false;
    mouseReleasedLeft=false;
    mousePressedRight=false;
    mouseReleasedRight=false;
    emit signalMouseDoubleClicked(mouseEvent);
}

void MyQLabel::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    if (mousePressedLeft==true)
    {
        mousePressedLeft=false;
        mouseReleasedLeft=true;
        mousePressedRight=false;
        mouseReleasedRight=false;
    }
    if (mousePressedRight==true)
    {
        mousePressedLeft=false;
        mouseReleasedLeft=false;
        mousePressedRight=false;
        mouseReleasedRight=true;

        // Set the rectangle defined while the user had right button pressed:
        rightClickRectangleReleased.setCoords(mousePressedPointRight.x(),mousePressedPointRight.y(),mouseEvent->pos().x(),mouseEvent->pos().y());

        // Send the chosen rectangle by the user with a signal:
        emit signalNewRectangle(rightClickRectangleReleased);
    }

    // Emit signal of mouse key release (independent of which mouse key):
    emit signalMouseReleased(mouseEvent);
}

void MyQLabel::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if (mousePressedRight==true)
        // If the user has pressed right mouse button while moving the mouse,...
    {
        // We want to send the rectangle whose one corner is when the user pressed right button and the other corner is where the mouse now is.
        rightClickRectangleMoved.setCoords(mousePressedPointRight.x(),mousePressedPointRight.y(),mouseEvent->pos().x(),mouseEvent->pos().y());

        // Emit the signal and send the rectangle defined by mouse movement:
        emit signalMouseMovedWithRightClickDown(rightClickRectangleMoved);
    }

    // Emit the signal of mouse movement (independent of mouse keys)...:
    emit signalMouseMoved(mouseEvent);

}

