Project Title
====
**A Graphical User Interface for Object Detection and Tracking in C++ With All Possible User Friendly Functionalities**

About
--
This is a Master Project of Mohammad Ashraful Alam, a student of **Information and Automation Engineering(IAE)** course, **Universität Bremen**, Germany.

The GUI had been developed at 25th October, 2017.

Prerequisites
--
Qt (cross-platform application framework): version 5.8.0

OpenCV (Open Source Computer Vision): version 2.9.0

MinGW (Cross Platform Makefile Generator): 32 bit

SQLite: version 3.20.1.0

All of the programs has been developed using OpenCV libraries and C++ Object Oriented Programming Language.

As a Cross Platform Application Framework, Qt had been chosen.

Description
--------
In this application, object detection and tracking can be done either on two **Images**(One for object detection and other one for tracking this detected object) or on a **Video** (where object has to be detected from i'th frame of video and detected object has to be tracked in (i+1)'th frame of this video).

**Using Image:** User has to choose two images (One for object detection and other one for tracking this detected object) from hard drive of local machine. The image for the object detection, will be then displayed on a QLabel named **Detection Image** and the other image for tracking this detected object, will be displayed on another QLabel called **Tracking Image**. On this detection image, user can apply a Haar Cascade Classifier detector for specific object detection. The classifier detector .xml file can be chosen from a pop-up menu named **Detector Selection**, a list of detector (*.xml*) files. After detector being chosen, "Detection Image" will automatically display the detected objects with rectagular box on this image. The user now needs to select an object bu **Mouse Clicking** which object he/she wants to track in the tracking image. For the tracking purpose,the user have to select a Feature detection technique from another pop-up menu named **Feature detection**, a list of feature detecting technique, by which user can track the previously selected object from the detection image.

 **Using Video:** When the user will select a video for object detection, the i'th frame and (i+1)'th frame of the video will be displayed on "Detection Image" and "Tracking Image" respectively. Then like the procedures has been done for the images, the same procedure has to be done for object detection and tracking. In this case, detector will detect object from i'th frame and then track the selected object from (i+1)'th frame.
 
**Detector (*.xml*) list:** In this project, as a object detector, **Haar Cascade Classifier** has been used. When OpenCV had been properly installed, a good number of haar cascade detector (*.xml*) files had also been included in OpenCV installation path (in my case, the path is *C:\opencv\sources\data\haarcascades\*)*.* **SQLite database system** has been, then, created to include all of the detector (*.xml*) files with their specific names. Then a connection has been stablished  between this database and pop-up menu **Detector Selection**. So when the user click on the **Detector Selection** pop-up menu, all of the detector will be displayed, which had been included in the time of OpenCV installation. 
 
There is an another **QPushButton** named **Add / Remove Detector**, located just below the **Detector Selection** pop-up menu. If the user like to add his/her owned trained classifier detector (*.xml*) files to the 'Detector Selection' pop-up menu or remove any specific detector (.xml) from the 'Detector Selection' pop-menu detector (*.xml*), then he/she will need to click the **Add / Remove Detector** pushbutton. Using this, user can add or remove any number of detector (*.xml*) file. Then the "Detector Selection" pop-up menu will also show the updated (newly added or removed) detector list, which can be now further used for object detection. 

