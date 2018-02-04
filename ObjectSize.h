#ifndef OBJECTSIZE_H
#define OBJECTSIZE_H

#include <QDialog>

namespace Ui {
class ObjectSize;
}

class ObjectSize : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectSize(QWidget *parent = 0);
    QString MinHeight, MinWidth, MaxHeight, MaxWidth;
    ~ObjectSize();

private slots:
    void on_Save_clicked();

    void on_Cancel_clicked();

private:
    Ui::ObjectSize *ui;
};

#endif // OBJECTSIZE_H
