//
// Created by Tomas Jasevicius
//

//screen size of iPhone5
//w 320 h 568
#pragma once

#include "ofxiOS.h"
#include "AVFoundationVideoGrabber.h"
#include "OrbTracker.hpp"
#include "OpticalFlowTracker.hpp"


class arApp : public ofxiOSApp {
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
    ////Old Orb Tracker implementation

//        //video recording/displaying
//        ofFpsCounter fps;
//        ofPixels tempPix;
//        AVFoundationVideoGrabber vidGrabber;
//        ofImage videoImg;
//        int counter; //counter
//
//    
//        //tracking
//        OrbTracker orbTracker;
//        ofImage markerImg;
//
//        bool bDetect;
//        bool bCreateHomography;
    ////
    
//    //OpticalFlowTracker opTracker;
//    
//    ofVideoGrabber vidGrabber;
//    ofImage videoImg;
//    int counter; //counter
//    
//    
//    //tracking
//    OrbTracker orbTracker;
//    ofImage markerImg;
//    
//    bool bDetect;
//    bool bCreateHomography;
//    
//    //optical flow thing
//    ofxCvGrayscaleImage pastImgGray;
//    ofxCvGrayscaleImage bgImgGray;
//    ofxCvGrayscaleImage currentImgGray;
//    ofxCvColorImage		currentImgColor;
//    
//    vector<cv::Point2f> points_keyPoints;
//    vector<cv::Point2f> points_nextPoints;
//    
//    bool once;
//    Mat bgImgGrayMat;
//    int state;
    
    ofVideoGrabber vidGrabber;
    ofImage videoImg;
    
    //ORBtracking
    OrbTracker orbTracker;
    ofImage markerImg;
    bool orbMagic;
    
    OpticalFlowTracker opticalFlow;
    bool flow;
    int state;
    
    
    


};


