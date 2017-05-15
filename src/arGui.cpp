//
//  arGui.cpp
//  VinylARt
//
//  Created by Tomas on 11/04/2017.
//
// Icons made using vector files found at Freepik http://www.freepik.com and Flaticon http://www.flaticon.com.
// They are licensed by Creative commons 3.0 http://creativecommons.org/licenses/by/3.0/ and were modified by Tomas Jasevicius.
//

#include "arGui.hpp"

//-----------------------------------------------------
void arGui::setup(){
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    //loading up images for icons, and menu tutorial
    next.load(ofToDataPath("images/icon-next.png"));
    next.resize(next.getWidth()/10, next.getHeight()/10);
    
    prev.load(ofToDataPath("images/icon-prev.png"));
    prev.resize(prev.getWidth()/10, prev.getHeight()/10);
    
    detect.load(ofToDataPath("images/icon-detect.png"));
    detect.resize(detect.getWidth()/6, detect.getHeight()/6);
    
    pause.load(ofToDataPath("images/icon-pause.png"));
    pause.resize(pause.getWidth()/6, pause.getHeight()/6);
    
    marker.load(ofToDataPath("images/detect-mark.png"));
    marker.resize(marker.getWidth()/1.7, marker.getHeight()/1.7);

    menu1.load(ofToDataPath("images/menu1.jpg"));
//    menu1.resize(menu1.getWidth()/3 ,menu1.getHeight()/3);
    
    menu2.load(ofToDataPath("images/menu2.jpg"));
    menu2.resize(menu2.getWidth()/2 ,menu2.getHeight()/2);
    
    menu3.load(ofToDataPath("images/menu3.jpg"));
    menu3.resize(menu3.getWidth()/2 ,menu3.getHeight()/2);
    
    menu4.load(ofToDataPath("images/menu4.jpg"));
    menu4.resize(menu4.getWidth()/2 ,menu4.getHeight()/2);
    
    menu5.load(ofToDataPath("images/menu5.jpg"));
    menu5.resize(menu5.getWidth()/2 ,menu5.getHeight()/2);
    
    menu6.load(ofToDataPath("images/menu6.jpg"));
    menu6.resize(menu6.getWidth()/2 ,menu6.getHeight()/2);
    
//    menu7.load(ofToDataPath("images/menu7.png"));
//    menu7.resize(menu7.getWidth()/2 ,menu7.getHeight()/2);
    
    menu8.load(ofToDataPath("images/menu8.png"));
    menu8.resize(menu8.getWidth()/2 ,menu8.getHeight()/2);


    fontBold.load(ofToDataPath("fonts/Gravity-Bold.otf"), 15, true, true);
    fontLight.load(ofToDataPath("fonts/Gravity-Regular.otf"), 15, true, true);
    
    //setting up button positions and states
    detectButton.set(ofGetScreenWidth()/2, 510);
    detectBtnRadius = 40;
    bDetectButton = false;
    
    nextSongButton.set(238, 488, 50, 50);
    bNextButton = false;
    
    prevSongButton.set(30, 488, 50, 50);
    bPrevButton = false;
    //positioning buttons
    tutorialButton.set(ofGetScreenWidth()/2+30, 490, 100, 50);
    startButton.set(30, 490, 100, 50);
    beginButton.set(ofGetScreenWidth()/2-50, 490, 100, 50);
    bBeginButton = false;
    
    prevMenuButton.set(0, 0, ofGetWidth()/2, ofGetHeight());
    nextMenuButton.set(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

    //setting up first screen position, and current screen for the menu
    firstScreenX = 0;
    currentScreen = 0;

}

//-----------------------------------------------------
// This function draws GUI such as buttons and information about the album
void arGui::draw(bool isPlaying, string name, int sideNo, ofImage *albumPic){
    
    //if music starts playing draw music control buttons and name of the song.
    if(isPlaying){
        //draw music controls
        drawMusicControls();
        //draw information about current sont
        drawSongName(name,albumPic);
        ofSetColor(255);
        //draw pause button
        pause.draw(ofGetScreenWidth()/2-43,470);
        //draw warning lines when detected image gets out of the sight
        drawBoundariesWarning(sideNo);
    }else if(!isPlaying){
        //draw middle detect button
        drawDetectButton();

    }

}
//-----------------------------------------------------
// This function draw artists name, song name and an album art cover at the top of the screen
void arGui::drawSongName(string name, ofImage *albumPic){
    
    //trim/split name into artist name and song name
    string artistName = ofSplitString(name,"-")[0];
    string songName = ofSplitString(name,"-")[1];
    //draw transparant square at the top of the screen
    //so that it would be easier to see the text
    ofSetColor(255,50);
    ofFill();
    ofDrawRectangle(0,0,ofGetScreenWidth(), 50);
    ofSetColor(255);
    //draw album picture
    albumPic->draw(5, 5, 40,40);
    ofSetColor(0);
    //draw song and artist names
    fontBold.drawString(songName, 50, 20);
    fontLight.drawString(" "+artistName, 50, 40);

}

//-----------------------------------------------------
// This function draws user feedback when the detect state is on
void arGui::drawDetectState(bool isDetect){
    if(isDetect){
        //draw the location marker on the screen
        //so that the users would know where to position the camera to
        ofSetColor(255,200);
        marker.draw(92,100);
        //draw yellow lines around the screen to indicate that detection is on
        ofSetColor(255,200,0);
        ofSetLineWidth(7);
        ofNoFill();
        ofDrawRectangle(80,36,ofGetWidth(),ofGetHeight());
    }
    
}

//-----------------------------------------------------
// This fucntion draws menu tutorial
void arGui::drawStartingScreen(){
    
    ofBackground(254, 220, 61);
    ofSetColor(255);
    //simple if statement to determine which tutorial screen to draw
    if(currentScreen==0){
        menu1.draw(0, 0,ofGetWidth(),ofGetHeight());
        drawButton(startButton,"Launch");
        drawButton(tutorialButton,"Tutorial");
    }else if (currentScreen==1){
        menu2.draw(0,0);
    }else if (currentScreen==2){
        menu3.draw(0,0);
    }else if (currentScreen==3){
        menu4.draw(0,0);
    }else if (currentScreen==4){
        menu5.draw(0,0);
    }else if (currentScreen==5){
        menu6.draw(0,0);
    }else if (currentScreen==6){
        menu8.draw(0,0);
        //when we are in the last screen, draw begin button so that the user
        //could get into the application
        drawButton(beginButton,"Begin");

    }
}

void arGui::drawButton(ofRectangle button,string btnText){
    ofFill();
    ofSetColor(254, 220, 61); //fill colour
    ofDrawRectangle(button);
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(0); //stroke
    //draw  button
    ofDrawRectangle(button);
    ofSetColor(0);
    if(button==startButton){
        fontBold.drawString(btnText,button.x+23,520);
    }else if(button==beginButton){
        fontBold.drawString(btnText,beginButton.x+30,520);
    }else{
        fontBold.drawString(btnText,button.x+20,520);
    }
}

//-----------------------------------------------------
// This function draws round detect button in the middle of the screen
void arGui::drawDetectButton(){
    ofSetColor(255);
    ofSetLineWidth(4);
    //set high resolution for the circle and draw it at the specified location
    ofSetCircleResolution(1000);
    detect.draw(ofGetScreenWidth()/2-43,470);
}

//-----------------------------------------------------
// This function draws music control buttons
void arGui::drawMusicControls(){
    //draw previous and next buttons using
    //positions stored in the variables initialised in setup
    next.draw(nextSongButton);
    prev.draw(prevSongButton);
    
}

//-----------------------------------------------------
// Listens for tap for the detect button
void arGui::checkDetectButton(ofTouchEventArgs & touch){
    //checking locations of touch on the screen for the detect button
    if (detectButton.distance(ofPoint(touch.x,touch.y)) < detectBtnRadius) {
        bDetectButton = !bDetectButton;
    }
    
}

//-----------------------------------------------------
// We use this function to detect when user taps, so we could switch between different tutorial screens
void arGui::checkMenuControls(ofTouchEventArgs & touch){
    
    if(!(currentScreen==6)){
    //checking locations of touch on the screen to move back
    if (prevMenuButton.inside(touch.x, touch.y)) {
            if(currentScreen>0){
                currentScreen--;
            }
    }
    
    if(!(currentScreen==0)){
    //checking locations of touch on the screen to move forwards
    if (nextMenuButton.inside(touch.x, touch.y)) {
            if(currentScreen<6){
                currentScreen++;
            }
    }
        }
        
    }
}

//-----------------------------------------------------
// We use this function to detect when user taps, so we could switch between different sound tracks
void arGui::checkMusicControls(ofTouchEventArgs & touch){
    //once the tap is detected we use boolean values to switch around
    //checking locations of touch on the screen for the music control buttons
    if (prevSongButton.inside(touch.x, touch.y)) {
        bPrevButton = !bPrevButton;
    }
    
    if (nextSongButton.inside(touch.x, touch.y)) {
        bNextButton = !bNextButton;
    }
}

//-----------------------------------------------------
// Listens for tap for the begin button
void arGui::checkBeginButton(ofTouchEventArgs & touch){
    
    //checking locations of touch on the screen for the start button
    if(currentScreen==0){
        if (startButton.inside(touch.x, touch.y)) {
            bBeginButton = !bBeginButton;
        }
        
        //checking locations of touch on the screen to move forwards
        if (tutorialButton.inside(touch.x, touch.y)) {
                currentScreen++;
        }
        
    }
    
    if(currentScreen==6){
    //checking locations of touch on the screen for the begin button
        if (beginButton.inside(touch.x, touch.y)) {
            bBeginButton = !bBeginButton;
        }
    }
}


//-----------------------------------------------------
// Here we draw lines around the screen to indicate the user that the tracked points are getting out of sight
void arGui::drawBoundariesWarning(int sideNo){
    
    ofSetColor(255,200,0,100);
    ofSetLineWidth(10);
    //we get these side numbers from OpticalFlowTracker
    //each of the side indicates sides of the screen, and so we can draw feedback to user if the points are going out of sight
    if(sideNo==1){
        ofDrawLine(0,0,0,ofGetScreenHeight());
    }else if(sideNo==2){
        ofDrawLine(ofGetScreenWidth(),0,ofGetScreenWidth(),ofGetScreenHeight());
    }else if(sideNo==3){
        ofDrawLine(0,0,ofGetScreenWidth(),0);
    }else if(sideNo==4){
        ofDrawLine(0,ofGetScreenHeight(),ofGetScreenWidth(),ofGetScreenHeight());
    }else if(sideNo==0){
        ofSetColor(255,0);
    }
    
}


//Set functions used to interact with other classes
//-----------------------------------------------------
void arGui::setbPrevButton(){
    bPrevButton=!bPrevButton;
}

//-----------------------------------------------------
void arGui::setbNextButton(){
    bNextButton=!bNextButton;
}

//-----------------------------------------------------
void arGui::setbDetectButton(){
    bDetectButton=!bDetectButton;
}

// Get functions used to interact with other classes
//-----------------------------------------------------
bool arGui::detectButtonPressed(){
    return bDetectButton;
}

//-----------------------------------------------------
bool arGui::nextButtonPressed(){
    return bNextButton;
}

//-----------------------------------------------------
bool arGui::prevButtonPressed(){
    return bPrevButton;
}

//-----------------------------------------------------
bool arGui::beginButtonPressed(){
    return bBeginButton;
}

//-----------------------------------------------------
int arGui::getCurrentScreen(){
    return currentScreen;
}
