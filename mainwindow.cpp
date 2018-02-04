#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <QMessageBox>
#include <QInputDialog>
#include "DetectorDatabase.h"
#include <QSqlError>
#include <Contents.h>
#include <About.h>
#include "ObjectSize.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Object Detection and Tracking");
    QPixmap pix("C:/Users/ASHRAF/Desktop/X-folder/headingLogo2.png");
    ui->detection_image->setPixmap(pix);
    ui->detection_image->setScaledContents(true);
    ui->detection_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->tracking_image->setScaledContents(true);
    ui->tracking_image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    //Adding Feature Detection Techniques to QComboBox "Features"
    ui->Features->addItem("Feature Selection");
    ui->Features->addItem("FAST");
    ui->Features->addItem("SURF");
    ui->Features->addItem("SIFT");
    ui->Features->addItem("ORB");
    ui->Features->addItem("BRIEF");
    ui->spinBox->setEnabled(false);

    //Here disabling the horizontalSlider for video play by false
    ui->horizontalSlider->setEnabled(false);

    //addDetector_to_pop_up_manu() function collects Detector .xml file from Database and add the to QComboBox "Detectors"
    addDetector_to_pop_up_manu();
    connect(this, SIGNAL(sendCurrentFrame(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
}

void MainWindow::addDetector_to_pop_up_manu()
{
    ui->Detectors->addItem("Detector Selection");

    //Connecting with SQLite database
    Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("C:/sqlite2/Detector.db");
    Database.open();
    query= new QSqlQuery(Database);
    QString queryString = "select * from DetectorList";
    query->prepare(queryString);

    if(!query->exec())
    {
        QMessageBox::critical(this,tr ("Error"),tr ("Error saving data"));
    }
    else{
        while (query->next()) {
            ui->Detectors->addItem(query->value(0).toString());
        }
    }
    Database.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{            
    exit(EXIT_FAILURE);
}

void MainWindow::on_RUN_clicked()
{
    getData_1 =ui->Features->currentText();

    if(getData_1=="Feature Selection" && getData_2=="Detector Selection")
    {
        if(!object.empty())
        {
            QMessageBox::warning(this, tr("Warning"),tr("Please select a specific Feature detection technique"));
        }
        else
        {
            QMessageBox::warning(this, tr("Warning"),tr("Please select a Feature and a specific Detector"));
        }
    }

    if(getData_1=="Feature Selection" && getData_2!="Detector Selection")
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please select a specific Feature detection technique"));
    }

    if(getData_1!="Feature Selection" && getData_2=="Detector Selection" && object.empty())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Please select a specific Detector"));
    }

    if(getData_1!="Feature Selection" && ( getData_2!="Detector Selection" || object.data))
    {
        if(!video_selection_path.empty())
        {
            if (!object.data)
            {
                QMessageBox::warning(this, tr("Warning"),tr("No object has been selected for object tracking. \n Please select a object first for tracking."));
            }
            else
            {
                DisplayImages();
                Feature_selection();
            }
        }
        else if (detection_image_path.empty())
        {
            QMessageBox::warning(this, tr("Warning"),tr("Neither Images nor Video has been chosen for object detection. \n Please choose first image or video for object detection"));
        }
        else if(!detection_image_path.empty() && tracking_image_path.empty())
        {
            QMessageBox::warning(this, tr("Warning"),tr("Image for tracking has not been chosen. \n Please choose a image for object tracking"));
        }
        else if (!object.data)
        {
            QMessageBox::warning(this, tr("Warning"),tr("No object has been selected for object tracking. \n Please select a object first for tracking."));
        }
        else
        {
            DisplayImages();
            Feature_selection();
        }
    }
}

void MainWindow::Feature_selection()
{        
    if(getData_1=="FAST")
    {
        FAST_feature_detection();
    }
    else if(getData_1=="SIFT")
    {
        SIFT_feature_detection();
    }
    else if(getData_1=="SURF")
    {
        SURF_feature_detection();
    }
    else if(getData_1=="BRIEF")
    {
        BRIEF_feature_detection();
    }
    else if(getData_1=="ORB")
    {
        ORB_feature_detection();
    }
}

void MainWindow::Classifier_selection()
{
    Database = QSqlDatabase::addDatabase("QSQLITE");
    Database.setDatabaseName("C:/sqlite2/Detector.db");
    Database.open();
    query= new QSqlQuery(Database);
    QString queryString = "select * from DetectorList";
    query->prepare(queryString);
    QString path;

    if(!query->exec())
    {
        QMessageBox::critical(this,tr ("Error"),tr ("Error saving data"));
    }
    while (query->next())
    {
        if (getData_2==query->value(0).toString())
        {
            path=query->value(1).toString().toLocal8Bit().constData();
            classifier=path.toLocal8Bit().constData();
            Classifier_Apply();
        }
    }
    Database.close();
}

void MainWindow::Classifier_Apply()
{    
    if(!ObjectWindowSize.MinHeight.isEmpty())
        MinHeight=ObjectWindowSize.MinHeight.toInt();
    else
        MinHeight=30;

    if(!ObjectWindowSize.MinWidth.isEmpty())
        MinWidth=ObjectWindowSize.MinWidth.toInt();
    else
        MinWidth=30;

    if(!ObjectWindowSize.MaxHeight.isEmpty())
        MaxHeight=ObjectWindowSize.MaxHeight.toInt();
    else
        MaxHeight=100;

    if(!ObjectWindowSize.MaxWidth.isEmpty())
        MaxWidth=ObjectWindowSize.MaxWidth.toInt();
    else
        MaxWidth=100;

    DisplayImages();
    if(frame.empty())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Detection image has not been selected. Please select a image or video for specific object detection."));
    }
    else if( !cascade.load( classifier ) ) //-- 1. Load the cascade
    {
        QMessageBox::warning(this, tr("Warning"),tr("Detector cannot be loaded. Please select a specific Detector and add it's path in the Database system."));
    }
    else if( !frame.empty() ) //-- 2. Apply the classifier to the frame
    { //detectAndDisplay( frame );

        cout<<MaxHeight<<endl<<MinWidth<<endl;

        Mat frame_gray;
        RNG rng(12345);

        Mat img = frame;
        cvtColor( frame, frame_gray, CV_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );

        Size Min(MinWidth, MinHeight), Max(MaxWidth, MaxHeight);

        //-- Detect faces
        cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Min, Max);

        size_t p= faces.size();
        cout<<"Faces: "<<p<<endl;

        for( size_t i = 0; i < faces.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            rectangle( frame, faces[i].tl() , faces[i].br(), color, 2, 8, 0 );
        }
        imwrite("C:/Users/ASHRAF/Desktop/test/object_detected_image.jpg",frame);        

        Mat3b src = frame;
        dest=Mat3b2QImage(src);
        //QPixmap imge=QPixmap::fromImage(dest);

        ui->detection_image->setPixmap(QPixmap::fromImage(dest));
        ui->detection_image->setScaledContents(true);
        ui->detection_image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

        QPoint point;
        //connect(ui->detection_img,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));

        connect(ui->detection_image,SIGNAL(signalMouseClicked(QMouseEvent*)),this,SLOT(slotMouseSingleClicked(QMouseEvent*)));
        connect(ui->detection_image,SIGNAL(signalMouseDoubleClicked(QMouseEvent*)),this,SLOT(slotMouseDoubleClicked(QMouseEvent*)));
    }
}

void MainWindow::DisplayImages()
{
    if(!detection_image_path.empty())
    {
        frame= imread(detection_image_path);
        frame2= imread(detection_image_path);
    }
    if(!tracking_image_path.empty())
    {
        frame3 =imread(tracking_image_path);
    }

    if(!video_selection_path.empty())
    {
        capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame);
        capture >> frame;
        capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame);
        capture >>frame2;
        capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame+1);
        capture >> frame3;
    }
}

void MainWindow::FAST_feature_detection()
{
    Mat3b inputImg, oututImg;
    inputImg=object;
    Mat GrayImage_1,GrayImage_2;

    if(inputImg.empty())
    {
        Classifier_Apply();
    }

    if(inputImg.channels() >2)
    {
        cvtColor( inputImg, GrayImage_1, CV_BGR2GRAY ); // converting color to gray image
    }
    else
    {
        GrayImage_1 =  inputImg;
    }

    oututImg=frame3;
    if(oututImg.channels() >2)
    {
        cvtColor( oututImg, GrayImage_2, CV_BGR2GRAY ); // converting color to gray image
    }
    else
    {
        GrayImage_2 =  oututImg;
    }

    vector<KeyPoint>keypoints1, keypoints2;

    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create("FAST");
    detector->detect(GrayImage_1, keypoints1, Mat());
    detector->detect(GrayImage_2, keypoints2, Mat());

    SurfDescriptorExtractor extractor;

    Mat descriptors_object, descriptors_scene;

    extractor.compute( GrayImage_1, keypoints1, descriptors_object );
    extractor.compute( GrayImage_2, keypoints2, descriptors_scene );

    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(descriptors_object, descriptors_scene, matches);

    Mat3b img_matches;
    drawMatches(inputImg, keypoints1, oututImg, keypoints2, matches, img_matches);

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(unsigned int i = 0; i < matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints1[ matches[i].queryIdx ].pt );
        scene.push_back( keypoints2[ matches[i].trainIdx ].pt );
    }

    if(obj.size()>3)
    {
        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( inputImg.cols, 0 );
        obj_corners[2] = cvPoint( inputImg.cols, inputImg.rows ); obj_corners[3] = cvPoint( 0, inputImg.rows );
        vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);


        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( inputImg.cols, 0), scene_corners[1] + Point2f( inputImg.cols, 0), Scalar(255, 255, 0), 4 );
        line( img_matches, scene_corners[1] + Point2f( inputImg.cols, 0), scene_corners[2] + Point2f( inputImg.cols, 0), Scalar( 255, 255, 0), 4 );
        line( img_matches, scene_corners[2] + Point2f( inputImg.cols, 0), scene_corners[3] + Point2f( inputImg.cols, 0), Scalar( 255, 255, 0), 4 );
        line( img_matches, scene_corners[3] + Point2f( inputImg.cols, 0), scene_corners[0] + Point2f( inputImg.cols, 0), Scalar( 255, 255, 0), 4 );
    }

    QImage pass=Mat3b2QImage(img_matches);
    display.DisplayQImage(pass);
    display.setModal(true);
    display.exec();
}

void MainWindow::SIFT_feature_detection()
{
    if(!featureData.nFeatures.isEmpty())
        nFeatures=featureData.nFeatures.toDouble();
    else
        nFeatures=0;
    if(!featureData.nOctaveLayers.isEmpty())
        nOctaveLayers=featureData.nOctaveLayers.toDouble();
    else
        nOctaveLayers=4;
    if(!featureData.edgeThreshold.isEmpty())
        edgeThreshold=featureData.edgeThreshold.toDouble();
    else
        edgeThreshold=10;
    if(!featureData.sigma.isEmpty())
        sigma=featureData.sigma.toDouble();
    else
        sigma=1.6;

    contrastThreshold=0.04;
    Mat inputImg,outputImg;
    inputImg = object;

    outputImg=frame3;
    cvtColor( outputImg, outputImg, CV_BGR2GRAY );



    // Detect keypoints in both images.
    FeatureDetector* detector;
    detector = new SiftFeatureDetector(
                nFeatures, //0
                nOctaveLayers, //4
                contrastThreshold, //0.001
                edgeThreshold, //2.9
                sigma //1.6
                );

    DescriptorExtractor* extractor;
    extractor = new SiftDescriptorExtractor();

    vector<KeyPoint> inputImgKeypoints,outputImgKeypoints;
    detector->detect(inputImg, inputImgKeypoints);
    detector->detect(outputImg, outputImgKeypoints);

    // Print how many keypoints were found in each image.

    Mat inputImgDescriptors, outputImgDescriptors;
    extractor->compute(inputImg, inputImgKeypoints, inputImgDescriptors);
    extractor->compute(outputImg, outputImgKeypoints, outputImgDescriptors);

    // Print some statistics on the matrices returned.
    Size size = inputImgDescriptors.size();

    size = outputImgDescriptors.size();

    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(inputImgDescriptors, outputImgDescriptors, matches);

    Mat3b img_matches;
    drawMatches(object, inputImgKeypoints, frame3, outputImgKeypoints, matches, img_matches);

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(unsigned int i = 0; i < matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( inputImgKeypoints[ matches[i].queryIdx ].pt );
        scene.push_back( outputImgKeypoints[ matches[i].trainIdx ].pt );
    }

    if(obj.size()>3)
    {
        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( inputImg.cols, 0 );
        obj_corners[2] = cvPoint( inputImg.cols, inputImg.rows ); obj_corners[3] = cvPoint( 0, inputImg.rows );
        vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);


        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( inputImg.cols, 0), scene_corners[1] + Point2f( inputImg.cols, 0), Scalar(0, 255, 255), 4 );
        line( img_matches, scene_corners[1] + Point2f( inputImg.cols, 0), scene_corners[2] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[2] + Point2f( inputImg.cols, 0), scene_corners[3] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[3] + Point2f( inputImg.cols, 0), scene_corners[0] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
    }

    QImage pass=Mat3b2QImage(img_matches);
    display.DisplayQImage(pass);
    display.setModal(true);
    display.exec();
}

void MainWindow::SURF_feature_detection()
{
    Mat img_object = object;
    cvtColor(img_object,img_object,cv::COLOR_RGB2GRAY);
    Mat img_scene = frame3;
    cvtColor(img_scene, img_scene, CV_BGR2GRAY);

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian;// = 5;

    if(!featureData.minHessian.isEmpty())
        minHessian=featureData.minHessian.toDouble();
    else
        minHessian=5;

    SurfFeatureDetector detector( minHessian );

    std::vector<KeyPoint> keypoints_object, keypoints_scene;

    detector.detect( img_object, keypoints_object );
    detector.detect( img_scene, keypoints_scene );

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descriptors_object, descriptors_scene;

    extractor.compute( img_object, keypoints_object, descriptors_object );
    extractor.compute( img_scene, keypoints_scene, descriptors_scene );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches);

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_object.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++)
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }

    Mat3b img_matches;
    drawMatches(object, keypoints_object, frame3, keypoints_scene,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Localize the object
    vector<Point2f> obj;
    vector<Point2f> scene;

    for(unsigned int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    if(obj.size()>3)
    {
        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
        obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
        vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);


        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    }
    //-- Show detected matches
    QImage pass=Mat3b2QImage(img_matches);
    display.DisplayQImage(pass);
    display.setModal(true);
    display.exec();
}

void MainWindow::BRIEF_feature_detection()
{
    Mat inputImg, grayImg_1, outputImg, grayImg_2;
    inputImg = object;
    outputImg=frame3;

    if(inputImg.channels()>2)
    {
        cvtColor(inputImg,grayImg_1, CV_BGR2GRAY);
    }
    else
    {
        grayImg_1=inputImg;
    }
    if(outputImg.channels()>2)
    {
        cvtColor(outputImg,grayImg_2, CV_BGR2GRAY);
    }
    else
    {
        grayImg_2=outputImg;
    }
    vector<KeyPoint>keypoints_1, keypoints_2;

    FastFeatureDetector detector;
    detector.detect(grayImg_1,keypoints_1);
    detector.detect(grayImg_2,keypoints_2);

    Mat img1_descriptor, img2_descriptor;
    BriefDescriptorExtractor extractor;
    extractor.compute(grayImg_1,keypoints_1, img1_descriptor );
    extractor.compute(grayImg_2,keypoints_2, img2_descriptor );

    Size size = img1_descriptor.size();

    size = img2_descriptor.size();

    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(img1_descriptor, img2_descriptor, matches);

    Mat3b img_matches;
    drawMatches(inputImg, keypoints_1, outputImg, keypoints_2, matches, img_matches);

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(unsigned int i = 0; i < matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_1[ matches[i].queryIdx ].pt );
        scene.push_back( keypoints_2[ matches[i].trainIdx ].pt );
    }

    if(obj.size()>3)
    {
        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( inputImg.cols, 0 );
        obj_corners[2] = cvPoint( inputImg.cols, inputImg.rows ); obj_corners[3] = cvPoint( 0, inputImg.rows );
        vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);


        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( inputImg.cols, 0), scene_corners[1] + Point2f( inputImg.cols, 0), Scalar(0, 255, 255), 4 );
        line( img_matches, scene_corners[1] + Point2f( inputImg.cols, 0), scene_corners[2] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[2] + Point2f( inputImg.cols, 0), scene_corners[3] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[3] + Point2f( inputImg.cols, 0), scene_corners[0] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
    }

    QImage pass=Mat3b2QImage(img_matches);
    display.DisplayQImage(pass);
    display.setModal(true);
    display.exec();
}

void MainWindow::ORB_feature_detection()
{    
    Mat inputImg, grayImg_1, outputImg, grayImg_2;
    Mat img1_descriptor, img2_descriptor;
    inputImg =object;
    outputImg=frame3;
    ORB orb;
    int method = 0;

    if(inputImg.channels()>2)
    {
        cvtColor(inputImg,grayImg_1, CV_BGR2GRAY);
    }
    else
    {
        grayImg_1=inputImg;
    }
    if(outputImg.channels()>2)
    {
        cvtColor(outputImg,grayImg_2, CV_BGR2GRAY);
    }
    else
    {
        grayImg_2=outputImg;
    }
    vector<KeyPoint>keypoints_1, keypoints_2;

    OrbFeatureDetector detector(25, 1.0f, 2, 10, 0, 2, 0, 10);
    OrbDescriptorExtractor extractor;
    if( method == 0 ) { //-- ORB
        orb.detect(grayImg_1, keypoints_1);
        orb.detect(grayImg_2, keypoints_2);
        orb.compute(grayImg_1, keypoints_1, img1_descriptor);
        orb.compute(grayImg_2, keypoints_2, img2_descriptor);
    } else { //-- SURF test
        detector.detect(grayImg_1, keypoints_1);
        detector.detect(grayImg_2, keypoints_2);
        extractor.compute(grayImg_1, keypoints_1, img1_descriptor);
        extractor.compute(grayImg_2, keypoints_2, img2_descriptor);
    }

    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(img1_descriptor, img2_descriptor, matches);

    Mat3b img_matches;
    drawMatches(inputImg, keypoints_1, outputImg, keypoints_2, matches, img_matches);

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(unsigned int i = 0; i < matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_1[ matches[i].queryIdx ].pt );
        scene.push_back( keypoints_2[ matches[i].trainIdx ].pt );
    }

    if(obj.size()>3)
    {
        Mat H = findHomography( obj, scene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        vector<Point2f> obj_corners(4);
        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( inputImg.cols, 0 );
        obj_corners[2] = cvPoint( inputImg.cols, inputImg.rows ); obj_corners[3] = cvPoint( 0, inputImg.rows );
        vector<Point2f> scene_corners(4);

        perspectiveTransform( obj_corners, scene_corners, H);

        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
        line( img_matches, scene_corners[0] + Point2f( inputImg.cols, 0), scene_corners[1] + Point2f( inputImg.cols, 0), Scalar(0, 255, 255), 4 );
        line( img_matches, scene_corners[1] + Point2f( inputImg.cols, 0), scene_corners[2] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[2] + Point2f( inputImg.cols, 0), scene_corners[3] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
        line( img_matches, scene_corners[3] + Point2f( inputImg.cols, 0), scene_corners[0] + Point2f( inputImg.cols, 0), Scalar( 0, 255, 255), 4 );
    }

    QImage pass=Mat3b2QImage(img_matches);
    display.DisplayQImage(pass);
    display.setModal(true);
    display.exec();
}

void MainWindow::DisplayDetectedObj()
{
    Point2f point3(point2.x(),point2.y());
    int x,y;

    x =static_cast<int>(float(point3.x)/float(ui->detection_image->width())*float(frame.cols));
    y =static_cast<int>(float(point3.y)/float(ui->detection_image->height())*float(frame.rows));

    Point2f pot(x,y);
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point2f p1,p2,p3,p4;
        p1= Point2f( faces[i].x, faces[i].y);
        p2= Point2f( faces[i].x + faces[i].width, faces[i].y);
        p3= Point2f( faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        p4= Point2f( faces[i].x , faces[i].y + faces[i].height);

        if(pot.x>p1.x && pot.x<p3.x && pot.y>p1.y && pot.y<p3.y)
        {
            object = frame2(faces[i]);
            imwrite("C:/Users/ASHRAF/Documents/object-detection-ui-master/object.jpg", object);
            namedWindow("object", CV_WINDOW_FREERATIO);
            imshow("object", object);
        }
    }
}

void MainWindow::on_actionDetect_triggered()
{
    video_selection_path.clear();
    ui->detection_image->clear();
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString imagefileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/ASHRAF/Desktop/", tr("Images (*.png *.xpm *.jpg)"));

    if(!imagefileName.isEmpty())
    {
        QImage image(imagefileName);
        ui->detection_image->setPixmap(QPixmap::fromImage(image));
        detection_image_path=imagefileName.toLocal8Bit().constData();
        ui->spinBox->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }
}

void MainWindow::on_actionVideo_triggered()
{
    tracking_image_path.clear();
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Videos (*.avi)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString videofileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("Videos (*.avi)"));

    if(!videofileName.isEmpty())
    {
        video_selection_path= videofileName.toLocal8Bit().constData();
        capture=VideoCapture(video_selection_path);
        if(!capture.isOpened())
        {
            QMessageBox::warning(this, tr("Warning"),tr("Error loadeing video."));
        }
        else
        {
            capture >> frame;
            if(frame.empty())
            {
                QMessageBox::warning(this, tr("Warning"),tr("Video frame cannot be openned."));
            }
            else
            {
                CurrentFrame=capture.get(CV_CAP_PROP_POS_FRAMES);
                int TotalFrame=capture.get(CV_CAP_PROP_FRAME_COUNT);
                ui->spinBox->setMaximum(TotalFrame);
                ui->horizontalSlider->setMaximum(ui->spinBox->maximum());

                emit sendCurrentFrame(CurrentFrame);

                image1= Mat3b2QImage(frame);
                ui->detection_image->setPixmap(QPixmap::fromImage(image1));
                QApplication::processEvents();

                capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame+1);
                capture >> frame3;
                image2= Mat3b2QImage(frame3);
                ui->tracking_image->setPixmap(QPixmap::fromImage(image2));
                QApplication::processEvents();
                capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame);

                ui->spinBox->setEnabled(true);
                ui->horizontalSlider->setEnabled(true);
            }
        }
    }
}
void MainWindow::on_actionTrack_triggered()
{
    ui->tracking_image->clear();
    video_selection_path.clear();
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString imagefileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/ASHRAF/Desktop/", tr("Images (*.png *.xpm *.jpg)"));

    if(!imagefileName.isEmpty())
    {
        QImage image(imagefileName);
        ui->tracking_image->setPixmap(QPixmap::fromImage(image));
        tracking_image_path=imagefileName.toLocal8Bit().constData();
        ui->spinBox->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }
}

void MainWindow::on_Detectors_currentIndexChanged(const QString &arg1)
{
    getData_2 = arg1;
    Classifier_selection();
}

void MainWindow::showMousePosition(QPoint &pos)
{    
    point2 = pos;
    DisplayDetectedObj();
}

QImage MainWindow::Mat3b2QImage (Mat3b src)
{
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for (int y = 0; y < src.rows; ++y) {
        const cv::Vec3b *srcrow = src[y];
        QRgb *destrow = (QRgb*)dest.scanLine(y);
        for (int x = 0; x < src.cols; ++x) {
            destrow[x] = qRgba(srcrow[x][2], srcrow[x][1], srcrow[x][0], 255);
        }
    }
    return dest;
}

void MainWindow::on_detector_clicked()
{
    DetectorDatabase object;
    object.setModal(true);
    object.exec();
    addDetector_to_pop_up_manu();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    CurrentFrame=arg1;
    capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame);
    capture >> frame;
    image1= Mat3b2QImage(frame);
    ui->detection_image->setPixmap(QPixmap::fromImage(image1));
    QApplication::processEvents();

    capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame+1);
    capture >> frame3;
    image2= Mat3b2QImage(frame3);
    ui->tracking_image->setPixmap(QPixmap::fromImage(image2));
    QApplication::processEvents();
    capture.set(CV_CAP_PROP_POS_FRAMES, CurrentFrame);
}

void MainWindow::on_actionContents_triggered()
{
    Contents c;
    c.setModal(true);
    c.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    About c;
    c.setModal(true);
    c.exec();
}

void MainWindow::on_pushButton_clicked()
{
    featureData.setModal(true);
    featureData.exec();
}

void MainWindow::on_Quit_clicked()
{
    exit(EXIT_FAILURE);
}

void MainWindow::slotMouseSingleClicked(QMouseEvent *mouseEvent)
{
    QString text;
    if (mouseEvent->button()==Qt::LeftButton)
        // If the left button of mouse is pressed,...
    {
        text="Left button clicked! x="+QString::number(mouseEvent->pos().x())+" ,y="+QString::number(mouseEvent->pos().y());
    }
    else if(mouseEvent->button()==Qt::RightButton)
        // If the right button of mouse is pressed,...
    {
        text="Right button clicked! x="+QString::number(mouseEvent->pos().x())+" ,y="+QString::number(mouseEvent->pos().y());
    }
    ui->label_2->setText(text);
    point2 = mouseEvent->pos();
    DisplayDetectedObj();
}

void MainWindow::slotMouseDoubleClicked(QMouseEvent *mouseEvent)
{
    QString text="Mouse double clicked! x="+QString::number(mouseEvent->pos().x())+" ,y="+QString::number(mouseEvent->pos().y());
    ui->label_2->setText(text);
}

void MainWindow::slotMouseMovedWithRightClickDown(QRect rectangle)
{
    DisplayImages();
    dest=Mat3b2QImage(frame2);
    QString text="Drawing rectangle,...";
    scaled=(QPixmap::fromImage(dest)).scaled(ui->detection_image->width(),ui->detection_image->height());
    ui->label_2->setText(text);
    QPainter paint;

    paint.begin(&scaled);
    paint.setBrush(Qt::blue);
    paint.setPen(Qt::blue);
    paint.setOpacity(0.4); // Between 0 and 1.

    paint.drawRect(rectangle);
    paint.end();

    ui->detection_image->setPixmap(scaled);
}

void MainWindow::slotNewRectangleReceived(QRect rectangle)
{
    QString text="New rectangle received! x="
            +QString::number(rectangle.x())+", y="
            +QString::number(rectangle.y())+", Width="
            +QString::number(rectangle.width())+", Height="
            +QString::number(rectangle.height());
    scaled=(QPixmap::fromImage(dest)).scaled(ui->detection_image->width(),ui->detection_image->height());
    ui->label_2->setText(text);
    QPainter paint;

    int x,y,width, height;
    x=static_cast<int>(float(rectangle.x())/float(ui->detection_image->width())*float(frame2.cols));
    y=static_cast<int>(float(rectangle.y())/float(ui->detection_image->height())*float(frame2.rows));
    width=static_cast<int>(float(rectangle.width())/float(ui->detection_image->width())*float(frame2.cols));
    height=static_cast<int>(float(rectangle.height())/float(ui->detection_image->height())*float(frame2.rows));

    Rect setectedRectangle =Rect(x,y, width, height);    
    object = frame2(setectedRectangle);

    if(!object.empty()){
        cvNamedWindow("Cropped Object", CV_WINDOW_FREERATIO);
        imshow("Cropped Object", object);
    }
    waitKey(0);
    paint.begin(&scaled);
    paint.setBrush(Qt::green);
    paint.setPen(Qt::green);
    paint.setOpacity(0.4); // Between 0 and 1.

    paint.drawRect(rectangle);
    paint.end();

    ui->detection_image->setPixmap(scaled);
}

void MainWindow::on_EnableCropObject_clicked()
{
    connect(ui->detection_image,SIGNAL(signalMouseMovedWithRightClickDown(QRect)),this,SLOT(slotMouseMovedWithRightClickDown(QRect)));
    connect(ui->detection_image,SIGNAL(signalNewRectangle(QRect)),this,SLOT(slotNewRectangleReceived(QRect)));

    //Now disconnecting other signals
    disconnect(ui->detection_image,SIGNAL(signalMouseClicked(QMouseEvent*)),this,SLOT(slotMouseSingleClicked(QMouseEvent*)));
    disconnect(ui->detection_image,SIGNAL(signalMouseDoubleClicked(QMouseEvent*)),this,SLOT(slotMouseDoubleClicked(QMouseEvent*)));
}

void MainWindow::on_AdjustObjSize_clicked()
{
    ObjectWindowSize.setModal(true);
    ObjectWindowSize.exec();
}
