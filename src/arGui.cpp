//
//  arGui.cpp
//  VinylAR2
//
//  Created by Tomas on 11/04/2017.
//
//

#include "arGui.hpp"

//-----------------------------------------------------
void arGui::setup(){
    
    //old OF default is 96 - but this results in fonts looking larger than in other programs.
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    

    detectButton.set(ofGetScreenWidth()/2, 510);
    detectBtnRadius = 40;
    bDetectButton = false;
    
    nextSongButton.set(240, 510, 50, 50);
    bNextButton = false;
    
    prevSongButton.set(30, 510, 50, 50);
    bPrevButton = false;
    
}

//-----------------------------------------------------
void arGui::draw(bool isPlaying,string name){
    //draw middle detect button
    drawDetectButton();

    //if music starts playing draw music control buttons and name of the song.
    if(isPlaying){
        drawMusicControls();
        drawSongName(name);
    }
    
}

//-----------------------------------------------------
void arGui::drawSongName(string name){
    ofSetColor(100,200);
    ofFill();
    ofDrawRectangle(0,0,ofGetScreenWidth(), 30);
    ofSetColor(0);
    verdana14.drawString(name, ofGetScreenWidth()*0.3, 20);
    ofSetColor(255);
    
}

//-----------------------------------------------------
void arGui::drawDetectState(bool isDetect){
    if(isDetect){
        ofSetColor(255,200,0);
        ofSetLineWidth(7);
        ofNoFill();
        ofDrawRectangle(80,36,ofGetWidth(),ofGetHeight());
    }
    
}

//-----------------------------------------------------
void arGui::drawStartingScreen(){
    
    ofBackground(0);
    ofSetColor(255);
    verdana14.drawString("Tap to begin", ofGetWidth()*0.4, ofGetHeight()/2);
    
}

//-----------------------------------------------------
void arGui::drawDetectButton(){
    
    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth(2);
    ofSetCircleResolution(1000);
    ofDrawCircle(detectButton, detectBtnRadius);
}

//-----------------------------------------------------
void arGui::drawMusicControls(){
    ofDrawRectangle(prevSongButton);
    ofDrawRectangle(nextSongButton);
}

//-----------------------------------------------------
void arGui::checkDetectButton(ofTouchEventArgs & touch){
    //checking locations of touch on the screen for the detect button
    if (detectButton.distance(ofPoint(touch.x,touch.y)) < detectBtnRadius) {
        bDetectButton = !bDetectButton;
    }
    
}

//-----------------------------------------------------
void arGui::checkMusicControls(ofTouchEventArgs & touch){
    //checking locations of touch on the screen for the music control buttons
    if (prevSongButton.inside(touch.x, touch.y)) {
        bPrevButton = !bPrevButton;
    }
    
    if (nextSongButton.inside(touch.x, touch.y)) {
        bNextButton = !bNextButton;
    }
}

//-----------------------------------------------------
void arGui::drawBoundariesWarning(int sideNo){
    
    ofSetColor(255,200,0,100);
    ofSetLineWidth(10);

    if(sideNo==1){
        ofDrawLine(0,0,0,ofGetScreenHeight());
    }
    
    if(sideNo==2){
        ofDrawLine(ofGetScreenWidth(),0,ofGetScreenWidth(),ofGetScreenHeight());
    }
    
    if(sideNo==3){
        ofDrawLine(0,0,ofGetScreenWidth(),0);
    }
    
    if(sideNo==4){
        ofDrawLine(0,ofGetScreenHeight(),ofGetScreenWidth(),ofGetScreenHeight());
    }
    
    

    
    
    
}



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
int arGui::getAppState(){
    return appState;
}
