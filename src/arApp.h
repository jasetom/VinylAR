//
// Created by Tomas Jasevicius
//

//screen size of iPhone5
//w 320 h 568
#pragma once

#include "ofxiOS.h"
#include "AVFoundationVideoGrabber.h"

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
    
        ofFpsCounter fps;
        //pixels copy to ofImage
        ofPixels p;
        AVFoundationVideoGrabber fGrabber;
        ofImage img;
        ofTrueTypeFont font;


};


