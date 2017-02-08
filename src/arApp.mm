//
// Created by Tomas Jasevicius
//


#include "arApp.h"

//--------------------------------------------------------------
void arApp::setup(){
    //Set orientation to portait
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    //innitiate video gravver
    fGrabber.setDevice(0);
    fGrabber.initGrabber(640,480);
    
}

//--------------------------------------------------------------
void arApp::update(){
    //update video grabber
    fGrabber.update();
    //if the frame is new create new picture out of pixels
    
    if(fGrabber.isFrameNew()){
        p.setFromPixels(fGrabber.getPixels(), fGrabber.getWidth(), fGrabber.getHeight(), fGrabber.getPixelFormat());
        img.setFromPixels(p);
        fps.newFrame();
    }
    
}

//--------------------------------------------------------------
void arApp::draw(){

    //draw image starting at minus coordinates so that the image
    //would be shown fully but it would show center
    img.draw(-60,-31);
    
}

//--------------------------------------------------------------
void arApp::exit(){

}

//--------------------------------------------------------------
void arApp::touchDown(ofTouchEventArgs & touch){
    ofBackground(233);

}

//--------------------------------------------------------------
void arApp::touchMoved(ofTouchEventArgs & touch){
    ofBackground(255,0,0);
    
}

//--------------------------------------------------------------
void arApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void arApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void arApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void arApp::lostFocus(){

}

//--------------------------------------------------------------
void arApp::gotFocus(){

}

//--------------------------------------------------------------
void arApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void arApp::deviceOrientationChanged(int newOrientation){

}
