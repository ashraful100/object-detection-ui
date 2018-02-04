#include "display_image.h"
#include "ui_display_image.h"

Display_Image::Display_Image(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Display_Image)
{
    ui->setupUi(this);
    this->setWindowTitle("Display Detected Image");

    ui->label->setScaledContents(true);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void Display_Image::DisplayQImage(QImage &src)
{
    ui->label->setPixmap(QPixmap::fromImage(src));
}

Display_Image::~Display_Image()
{
    delete ui;
}

void Display_Image::on_Ok_clicked()
{
    close();
}
