//
// Created by Tomas Jasevicius
//


#include "arApp.h"

//--------------------------------------------------------------
void arApp::setup(){
    
    CGSize ARViewBoundsSize = CGSizeZero;
    UIInterfaceOrientation ARViewOrientation = UIInterfaceOrientationUnknown;
    
    ARViewBoundsSize.width = [[UIScreen mainScreen] bounds].size.width;
    ARViewBoundsSize.height = [[UIScreen mainScreen] bounds].size.height;
    ARViewOrientation = UIInterfaceOrientationPortrait;

    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    
    //basic setup for programm
    vidGrabber.initGrabber(640,480);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
     
    
    ////OrbTracker 0 pebbles
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/1.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 1 deer
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/2.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 2 man
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/3.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 3 wall
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/4.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 4 traintracks
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/5.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 5 roof
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/6.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    
    ////OrbTracker 6 car
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/7.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);

    //once we analyse all of the images we train our matcher with descriptors
    orbTracker.trainMatches(orbTracker.getManyImgDescriptors());

    
    ////OpticalFlowTracker
    opticalFlow.setup(vidGrabber.getWidth(),vidGrabber.getHeight());
    
    ////Sound setup
    //setup Music Manager
    musicMan.setup();

    //left and right audio output sizes
    initialBufferSize = 512;
    lAudio = new float[initialBufferSize];
    rAudio = new float[initialBufferSize];
    
    detectButton.set(ofGetScreenWidth()/2, 510);
    detectBtnRadius = 40;
    bDetectButton = false;
    
    nextSongButton.set(240, 510, 50, 50);
    bNextButton = false;
    
    prevSongButton.set(30, 510, 50, 50);
    bPrevButton = false;
    
}

//--------------------------------------------------------------
void arApp::update(){
    
    vidGrabber.update();
    if(vidGrabber.isFrameNew()){
        
        //we tap screen and this all happens.
        if(orbMagic==true){
            //if we dont have any boundaries yet, we run the detection function
            if(orbTracker.getBoundariesKeyPoints().size()<3){
                orbTracker.detect(vidGrabber.getPixels().getData(),vidGrabber.getWidth(),vidGrabber.getHeight());
            }else{
                //once we have more than 3 keypoints in boundaries we move to optical flow tracking
                flow = true;
            }
            
            if(orbTracker.match()>1){
            //we create homography with detected img number. we do -1 as to access correct vectors for the images
                orbTracker.createHomography(orbTracker.getImgKeyPoints(orbTracker.getDetectedImgNumber()-1),orbTracker.getImgBoundaries(orbTracker.getDetectedImgNumber()-1));
                
                //we call firstDetect function in music player to change some variables
                musicMan.firstDetect();
                
            }
        }
        
        if(flow==true){
            //when we have 4 points in boundaries we pass them to optical flow function for easy tracking
            opticalFlow.updateFlowImage(vidGrabber.getPixels().getData(),orbTracker.getBoundariesKeyPoints(),orbMagic);
            //disabling detection/tracking/matching from orbTracker
            orbMagic = false;
            musicMan.play(true);

        }
    }
    
    musicMan.update(orbTracker.getDetectedImgNumber());
    
    
    //check buttons
    if (bDetectButton){
        orbMagic =!orbMagic;
        flow = false;
        orbTracker.reset();
        musicMan.play(false);
        bDetectButton=!bDetectButton;
    }
    
        if (bPrevButton){
            musicMan.prevSong();
            bPrevButton =!bPrevButton;
        }
        if (bNextButton){
            musicMan.nextSong();
            bNextButton=!bNextButton;
        }
    
    
    
    
    
}

    


//--------------------------------------------------------------
void arApp::draw(){
    
    //iphone se size 568 x 320
    //camera img size 640 x 480
    //therefore we center it out using translate
    ofPushMatrix();
    ofTranslate(-80,-36);

    ofSetColor(255);
    vidGrabber.draw(0,0);
    
    ofSetColor(0);
    ofDrawBitmapString(ofGetWidth(), 20, 20);
    ofDrawBitmapString(ofGetHeight(), 20, 40);
    
    orbTracker.draw();
    
    //homography was here
    
    if(orbMagic){
        ofSetColor(255,200,0);
        ofSetLineWidth(7);
        ofNoFill();
        ofDrawRectangle(80,36,ofGetWidth(),ofGetHeight());
    }
    
    //here we would draw sound
    if(flow){
                
        if(opticalFlow.trackingPointsVisible()){
            
            
            //draw op homography
            opticalFlow.drawHomography();
            
            
//            ofSetColor(0,255,0);
//            ofSetLineWidth(7);
//            ofNoFill();
//            ofDrawRectangle(80,36,ofGetWidth(),ofGetHeight());
            
            if(orbTracker.getDetectedImgNumber()==1){
                ofDrawBitmapString("Pebbles! "+ musicMan.getCurrentSongName(),opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
                //                                ofDrawSphere(opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10,opticalFlow.getDrawingScalar());
                
                if(musicMan.isPlay()){
                    //
                    musicMan.draw(opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y);
                    
                }
            }
            
            if(orbTracker.getDetectedImgNumber()==2){
                ofDrawBitmapString("Man!",opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
            }
            if(orbTracker.getDetectedImgNumber()==3){
                ofDrawBitmapString("Wall!",opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
            }
            if(orbTracker.getDetectedImgNumber()==4){
                ofDrawBitmapString("Train tracks!",opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
            }
            if(orbTracker.getDetectedImgNumber()==5){
                ofDrawBitmapString("Roof!",opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
            }
            if(orbTracker.getDetectedImgNumber()==6){
                   ofDrawBitmapString("Car!",opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y,10);
            }
            
        }else{
            musicMan.play(false);
        }
        
        //preparation for visualisations implementation
//        ofBeginShape();
//        ofSetColor(255,133,133);
//        ofFill();
//        
//        for(int i=0; i < fftSize / 8; i++) {
//        
//            ofSetColor(7*i,133,133);
//            //place spheres in a circle using trig functions
//            int x = cos(i)*100;
//            int y = sin(i)*100;
//            ofDrawSphere(x,y,150,5+mfft.magnitudes[i]*2);
//        }
//        ofEndShape();
    }
    
    ofPopMatrix();
    //drawing gui on top of everything
    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth(2);
    ofSetCircleResolution(1000);
    ofDrawCircle(detectButton, detectBtnRadius);
    if(musicMan.isPlay()){
        ofDrawRectangle(prevSongButton);
        ofDrawRectangle(nextSongButton);
    }
   
}

//--------------------------------------------------------------
void arApp::exit(){

}

//--------------------------------------------------------------
void arApp::touchDown(ofTouchEventArgs & touch){
    
    //checking locations of touch on the screen for the buttons
    if (detectButton.distance(ofPoint(touch.x,touch.y)) < detectBtnRadius) {
        bDetectButton = !bDetectButton;
    }

    //check for buttons only when points are visible
    if(opticalFlow.trackingPointsVisible()){

        if (prevSongButton.inside(touch.x, touch.y)) {
            bPrevButton = !bPrevButton;
        }
    
        if (nextSongButton.inside(touch.x, touch.y)) {
            bNextButton = !bNextButton;
        }
    }
    

}

//--------------------------------------------------------------
void arApp::touchMoved(ofTouchEventArgs & touch){
    
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
//--------------------------------------------------------------
void arApp::audioOut(float * output, int bufferSize, int nChannels) {
    //audio output
    musicMan.audioOut(output,bufferSize,nChannels,orbTracker.getDetectedImgNumber());

}
