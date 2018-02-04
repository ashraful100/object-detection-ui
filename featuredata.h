#ifndef FEATUREDATA_H
#define FEATUREDATA_H

#include <QDialog>

namespace Ui {
class FeatureData;
}

class FeatureData : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureData(QWidget *parent = 0);
    QString nFeatures,nOctaveLayers,contrastThreshold,edgeThreshold,sigma,minHessian;
    ~FeatureData();

private slots:

    void on_Save_clicked();

    void on_Cancel_clicked();

private:
    Ui::FeatureData *ui;
};

#endif // FEATUREDATA_H
