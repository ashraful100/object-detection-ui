#ifndef DISPLAY_IMAGE_H
#define DISPLAY_IMAGE_H

#include <QDialog>
#include <opencv/highgui.h>
using namespace cv;

namespace Ui {
class Display_Image;
}

class Display_Image : public QDialog
{
    Q_OBJECT

public:
    explicit Display_Image(QWidget *parent = 0);
    void DisplayQImage(QImage &src);
    ~Display_Image();

private slots:
    void on_Ok_clicked();

private:
    Ui::Display_Image *ui;
};

#endif // DISPLAY_IMAGE_H
