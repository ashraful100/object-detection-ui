#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>

class MyQLabel : public QLabel
{
    Q_OBJECT // A macro. Needed to work with signals and slots. Clean all build and run qmake right after adding this to avoid error!
public:
    explicit MyQLabel(QWidget* parent = 0);

    bool mousePressedRight;
    bool mouseReleasedRight;
    bool mousePressedLeft;
    bool mouseReleasedLeft;

    QPoint mousePressedPointLeft;
    QPoint mousePressedPointRight;

    QRect rightClickRectangleMoved;
    QRect rightClickRectangleReleased;

signals:
    void signalMouseClicked(QMouseEvent *mouseEvent);
    void signalMouseDoubleClicked(QMouseEvent *mouseEvent);
    void signalMouseReleased(QMouseEvent *mouseEvent);
    void signalMouseMoved(QMouseEvent *mouseEvent);
    void signalNewRectangle(QRect rectangle);
    void signalMouseMovedWithRightClickDown(QRect rectangle);

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
};

#endif // MYQLABEL_H
