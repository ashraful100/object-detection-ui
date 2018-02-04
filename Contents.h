#ifndef CONTENTS_H
#define CONTENTS_H

#include <QDialog>

namespace Ui {
class Contents;
}

class Contents : public QDialog
{
    Q_OBJECT

public:
    explicit Contents(QWidget *parent = 0);
    ~Contents();

private:
    Ui::Contents *ui;
};

#endif // CONTENTS_H
