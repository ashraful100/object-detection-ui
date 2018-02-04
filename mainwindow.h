#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include<QFileDialog>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <featuredata.h>
#include <display_image.h>
#include "ObjectSize.h"

#include <QPixmap>
#include <QPainter>

#include <QPoint>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Mat object;
    explicit MainWindow(QWidget *parent = 0);   
    QSqlDatabase Database;
    Display_Image display;
    QSqlQuery *query;
    int frameNumber;int CurrentFrame;
    VideoCapture capture;   
    QImage image1, image2;
    FeatureData featureData;
    ObjectSize ObjectWindowSize;

    QPixmap scaled;
    QImage dest;

    void FAST_feature_detection();
    void SIFT_feature_detection();
    void SURF_feature_detection();
    void BRIEF_feature_detection();
    void ORB_feature_detection();
    void addDetector_to_pop_up_manu();

    void DisplayDetectedObj();
    void Classifier_selection();
    void Feature_selection();
    void Classifier_Apply();
    void DisplayImages();

    QImage Mat3b2QImage (Mat3b src);    


    double nFeatures,nOctaveLayers,contrastThreshold,edgeThreshold,sigma;
    int MinHeight, MinWidth, MaxHeight, MaxWidth;

    vector<Rect> faces;
    vector<Mat>collect;
    Mat3b frame, frame2, object_detected_image, frame1,frame3, result_image;
    QImage image;
    QString getData_1,getData_2;
    QPoint point2;
    CascadeClassifier cascade;
    std::string classifier, detection_image_path,tracking_image_path, video_selection_path;
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionDetect_triggered();

    void on_actionVideo_triggered();

    void on_actionTrack_triggered();

    void on_RUN_clicked();       

    void on_detector_clicked();

    void on_Detectors_currentIndexChanged(const QString &arg1);    

    void on_spinBox_valueChanged(int arg1);

    void on_actionContents_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_clicked();

    void on_Quit_clicked();

    void on_EnableCropObject_clicked();

    void on_AdjustObjSize_clicked();

private:
    Ui::MainWindow *ui;

public slots:
    void slotMouseSingleClicked(QMouseEvent *mouseEvent);
    void slotMouseDoubleClicked(QMouseEvent *mouseEvent);
    //void slotMouseReleased(QMouseEvent *mouseEvent);
    //void slotMouseMoved(QMouseEvent *mouseEvent);
    void slotMouseMovedWithRightClickDown(QRect rectangle);
    void slotNewRectangleReceived(QRect rectangle);

    void showMousePosition(QPoint& pos);
signals:
    void sendCurrentFrame(int);
};

#endif // MAINWINDOW_H
