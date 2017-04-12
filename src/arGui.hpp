//
//  arGui.hpp
//  VinylAR2
//
//  Created by Tomas on 11/04/2017.
//
//

#ifndef arGui_hpp
#define arGui_hpp

#include <stdio.h>
#include "ofMain.h"

class arGui{
    
public:

    void setup();
    void draw(bool isPlaying, string name);
    void drawSongName(string name);
    void drawStartingScreen();
    void drawDetectButton();
    void drawMusicControls();
    void drawDetectState(bool isDetect);
    
    void checkDetectButton(ofTouchEventArgs & touch);
    void checkMusicControls(ofTouchEventArgs & touch);
    
    void drawBoundariesWarning(int sideNo);
    
    bool detectButtonPressed();
    bool nextButtonPressed();
    bool prevButtonPressed();
    void setbPrevButton();
    void setbNextButton();
    void setbDetectButton();



    
    int getAppState();
    
    
    /* Basic GUI */
    ofTrueTypeFont verdana14;
    int appState;
    ofPoint detectButton;
    bool bDetectButton;
    int detectBtnRadius;
    
    ofRectangle prevSongButton;
    bool bPrevButton;
    ofRectangle nextSongButton;
    bool bNextButton;

    
};



#endif /* arGui_hpp */
