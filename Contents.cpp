#include "Contents.h"
#include "ui_contents.h"
#include <QFile>
#include<QMessageBox>
#include<QTextStream>

Contents::Contents(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Contents)
{
    ui->setupUi(this);

    this->setWindowTitle("Contents");

    //Opening the About text file
    QFile file("C:/Users/ASHRAF/Documents/object-detection-ui-master/Contents.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"Info",file.errorString());
    }
    QTextStream In(&file);
    //Setting text of About in our text browser

    ui->textBrowser->setText(In.readAll());
}

Contents::~Contents()
{
    delete ui;
}
