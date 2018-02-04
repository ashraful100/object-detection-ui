#include "About.h"
#include "ui_about.h"
#include <QFile>
#include<QMessageBox>
#include<QTextStream>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    this->setWindowTitle("About");

    //Opening the About text file
    QFile file("C:/Users/ASHRAF/Documents/object-detection-ui-master/About.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"Info",file.errorString());
    }
    QTextStream In(&file);
    //Setting text of About in our text browser
    ui->textBrowser->setText(In.readAll());
}

About::~About()
{
    delete ui;
}
