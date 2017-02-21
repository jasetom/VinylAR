//
// Created by Tomas Jasevicius
//

//screen size of iPhone5
//w 320 h 568
#pragma once

#include "ofxiOS.h"
#include "AVFoundationVideoGrabber.h"
#include "ofxSURFTracker.h"
#include "OrbTracker.hpp"


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
    
        //video recording/displaying
        ofFpsCounter fps;
        ofPixels tempPix;
        AVFoundationVideoGrabber vidGrabber;
        ofImage videoImg;
        int counter; //counter

    
        //tracking
        OrbTracker orbTracker;
//        ofxSURFTracker surfTracker;
        ofImage markerImg;

        bool bDetect;
        bool bCreateHomography;


};


