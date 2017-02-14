//
// Created by Tomas Jasevicius
//


#include "arApp.h"

//--------------------------------------------------------------
void arApp::setup(){
    
//    ofSetVerticalSync(true);
    //Set orientation to portait
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    ofSetVerticalSync(true);
    ofSetFrameRate(25);
    //innitiate video grabber
    vidGrabber.setDevice(0);
    
    vidGrabber.initGrabber(40,80);
    
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/marker1.jpg"));
    
    //analyse image to get its features
    surfTracker.analyseImage(markerImg);
    surfTracker.createBounds();
//    surfTracker.DrawMatches(false);
//    ofSetFrameRate(20);
    //init counter
    counter=0;
    


}

//--------------------------------------------------------------
void arApp::update(){
    //update video grabber
    vidGrabber.update();
//    //if the frame is new create new picture out of pixels
//    
    if(vidGrabber.isFrameNew()){
        counter++;

//        //create new video image
        tempPix.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), vidGrabber.getPixelFormat());
        videoImg.setFromPixels(tempPix);
//        fps.newFrame();
        
        //
        
        if(bDetect){
            if(counter%3==0){
            //do detection of the features on video grabber
                surfTracker.detect(vidGrabber.getPixels(),vidGrabber.getWidth(), vidGrabber.getHeight());

            }
            if(surfTracker.match(surfTracker.getObjectKeyPoints(),surfTracker.getObjectDescriptors(),surfTracker.getObjectBounds())!=0){
                //
                surfTracker.createHomography(surfTracker.getObjectKeyPoints(),surfTracker.getObjectBounds());
                //
            }

        }
    }
        
    
}
    


//--------------------------------------------------------------
void arApp::draw(){
//
//    //draw image starting at minus coordinates so that the image
//    //would be shown fully but it would show center
//    ofPushMatrix();
//    ofTranslate(-60,-31);
    videoImg.draw(0,0);
//    ofPopMatrix();
////    // draw the tracking image on top of the camera image
////    if(bCreateHomography){
    surfTracker.draw();

    
}

//--------------------------------------------------------------
void arApp::exit(){

}

//--------------------------------------------------------------
void arApp::touchDown(ofTouchEventArgs & touch){
    bDetect =true;

}

//--------------------------------------------------------------
void arApp::touchMoved(ofTouchEventArgs & touch){
    ofBackground(0,0,0);
    
}

//--------------------------------------------------------------
void arApp::touchUp(ofTouchEventArgs & touch){
    bDetect =false;


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
