//
//  arGui.hpp
//  VinylARt
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
    //basic function declarations
    void setup();
    void draw(bool isPlaying, string name, int sideNo, ofImage *albumPic);
    void drawSongName(string name, ofImage *albumPic);
    void drawStartingScreen();
    void drawButton(ofRectangle button,string btnText);
    void drawDetectButton();
    void drawMusicControls();
    void drawDetectState(bool isDetect);
    void drawBoundariesWarning(int sideNo);

    //touch detection functions
    void checkDetectButton(ofTouchEventArgs & touch);
    void checkMusicControls(ofTouchEventArgs & touch);
    void checkBeginButton(ofTouchEventArgs & touch);
    void checkPauseButton(ofTouchEventArgs & touch);
    void checkMenuControls(ofTouchEventArgs & touch);

    //geters
    bool detectButtonPressed();
    bool isPause();
    bool pauseButtonPressed();
    bool nextButtonPressed();
    bool prevButtonPressed();
    int getCurrentScreen();
    bool beginButtonPressed();

    //setters
    void setbPrevButton();
    void setbNextButton();
    void setbDetectButton();
    void setbPauseButton();
    
    /* Basic GUI variables */
    ofTrueTypeFont fontBold, fontLight;
    //button variables
    ofImage prev, next, detect, pause, marker;
    ofPoint detectButton;
    bool bDetectButton;
    int detectBtnRadius;
    ofRectangle prevSongButton;
    bool bPrevButton;
    ofRectangle nextSongButton;
    bool bNextButton;
    bool bPauseButton;
    
    //menu variables
    ofImage menu1, menu2, menu3, menu4, menu5, menu6, menu7, menu8;
    int currentScreen;
    int firstScreenX, secondScreenX, thirdScreenX, fourthScreenX, fifthScreenX, sixthScreenX, seventhScreenX;
    ofRectangle beginButton, startButton, tutorialButton;
    ofRectangle prevMenuButton, nextMenuButton;
    bool bPrevMenuButton, bNextMenuButton;
    bool bBeginButton;

    int appState; //aplication state variable for gui
};



#endif /* arGui_hpp */
