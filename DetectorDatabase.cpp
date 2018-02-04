#include "DetectorDatabase.h"
#include "ui_detectordatabase.h"
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSqlTableModel>
#include <iostream>

DetectorDatabase::DetectorDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetectorDatabase)
{
    ui->setupUi(this);
    this->setWindowTitle("Detector Database List");

    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName("C:/sqlite2/Detector.db");

    if(!mDatabase.open()){
        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
        return;
    }

    qry = new QSqlQuery(mDatabase);
    qry->prepare("select * from DetectorList");
    qry->exec();

    kModel = new QSqlTableModel(this);
    kModel->setTable("DetectorList");
    kModel->select();
    ui->tableView->setModel(kModel);
    ui->tableView->setVisible(false);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setVisible(true);

}

DetectorDatabase::~DetectorDatabase()
{
    delete ui;
}

void DetectorDatabase::on_Add_clicked()
{
    kModel->insertRow(kModel->rowCount());
    decision=true;
}

void DetectorDatabase::on_Remove_clicked()
{
    kModel->removeRow(ui->tableView->currentIndex().row());
    ui->tableView->setModel(kModel);
    ui->tableView->setVisible(false);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setVisible(true);
    decision=false;
}

void DetectorDatabase::on_Update_clicked()
{
    if(decision)
    {
        unsigned int rows=0;
        while (qry->next())
        {
            rows++;
        }

        QString name= ui->tableView->model()->data(ui->tableView->model()->index(rows,0)).toString();
        QString path= ui->tableView->model()->data(ui->tableView->model()->index(rows,1)).toString();

        qry->prepare("INSERT INTO DetectorList (NAME,PATH) VALUES ('"+name+"','"+path+"')");

        if(qry->exec())
        {
            ui->tableView->setModel(kModel);
            ui->tableView->setVisible(false);
            ui->tableView->resizeColumnsToContents();
            ui->tableView->setVisible(true);

            QMessageBox::information(this,tr ("Save"),tr ("Saved"));
        }
        else
        {
            QMessageBox::critical(this,tr ("Error"),tr ("Error saving data"));
        }

    }
    else
    {
        QMessageBox::information(this,tr ("Save"),tr ("Saved"));
    }
    close();
}
