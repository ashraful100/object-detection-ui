#ifndef DETECTORDATABASE_H
#define DETECTORDATABASE_H

#include <QDialog>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>

namespace Ui {
class DetectorDatabase;
}
class QSqlTableModel;

class DetectorDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit DetectorDatabase(QWidget *parent = 0);
    QSqlDatabase mDatabase;
    QSqlQuery *qry;
    ~DetectorDatabase();

private slots:
    void on_Add_clicked();

    void on_Remove_clicked();

    void on_Update_clicked();

private:
    bool decision=false;
    Ui::DetectorDatabase *ui;    
    QSqlTableModel *kModel;

};

#endif // DETECTORDATABASE_H
