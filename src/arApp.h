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
#include "ofxMaxim.h"
#include "MusicManager.hpp"


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
    
    /* Tracking */
    ofVideoGrabber vidGrabber;
    ofImage videoImg;
    //ORBtracking
    OrbTracker orbTracker;
    ofImage markerImg;
    bool orbMagic;
    //OpticalFlowTracking
    OpticalFlowTracker opticalFlow;
    bool flow;
    int state;
    
    /* Sound stuff */
    void audioOut(float * output, int bufferSize, int nChannels); //audio output function
    
    /* Music player */
    MusicManager musicMan;
    double playingSound; //variable where we store data when sound is playing
    
    int initialBufferSize;
    float * lAudio; //left audio output
    float * rAudio; //right audio output
    
    /* Basic GUI */
    ofPoint detectButton;
    bool bDetectButton;
    int detectBtnRadius;
    
    ofRectangle prevSongButton;
    bool bPrevButton;
    ofRectangle nextSongButton;
    bool bNextButton;

    


};


