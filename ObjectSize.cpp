#include "ObjectSize.h"
#include "ui_ObjectSize.h"

ObjectSize::ObjectSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectSize)
{
    ui->setupUi(this);
    this->setWindowTitle("Adjust Detection Parameter");

    ui->spinBox->setMaximum(500);
    ui->spinBox_2->setMaximum(500);
    ui->spinBox_3->setMaximum(500);
    ui->spinBox_4->setMaximum(500);
}

ObjectSize::~ObjectSize()
{
    delete ui;
}

void ObjectSize::on_Save_clicked()
{
    MinHeight= ui->spinBox->text();
    MinWidth = ui->spinBox_2->text();
    MaxHeight= ui->spinBox_3->text();
    MaxWidth = ui->spinBox_4->text();

    close();
}

void ObjectSize::on_Cancel_clicked()
{
    MinHeight.clear();
    MinWidth.clear();
    MaxHeight.clear();
    MaxWidth.clear();

    ui->spinBox->clear();
    ui->spinBox_2->clear();
    ui->spinBox_3->clear();
    ui->spinBox_4->clear();

    close();
}
