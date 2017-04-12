//
// Created by Tomas Jasevicius
//


#include "arApp.h"

//--------------------------------------------------------------
void arApp::setup(){
    //Initial setup for the programm
    //we use this to keep track the state of the application
    appState = 0;
    //aliasing and smoothing
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    //innitialise camera feed
    vidGrabber.initGrabber(640,480);
    //vertical sync on, framerate 60fps
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    ////OrbTracker setup
    orbTracker.setup();
    
    ////OpticalFlowTracker setup
    opticalFlow.setup(vidGrabber.getWidth(),vidGrabber.getHeight());
    
    ////Sound setup
    musicMan.setup();
    
    ////Gui setup
    gui.setup();

    ////Visuals setup
    vizMan.setup();
}

//--------------------------------------------------------------
void arApp::update(){
    
    //if appState=1 this means that we are in the main state of the app
    if(appState==1){
    //update video feed
    vidGrabber.update();
    //when we recieve new frame from camera feed, do the following
    if(vidGrabber.isFrameNew()){
        
        //if detect button has been pressed, orbMagic becomes true run orbTracker.
        if(orbMagic==true){
            //if we dont have any boundaries yet, we run the detection function
            if(orbTracker.getBoundariesKeyPoints().size()<3){
                //take pixels from the camera and try to detect pre-analysed images in the camera feed.
                orbTracker.detect(vidGrabber.getPixels().getData(),vidGrabber.getWidth(),vidGrabber.getHeight());
            }else{
                //once we have more than 3 keypoints in boundaries, that means that we have detected an image
                //therefore we can move to optical flow tracking
                flow = true;
            }
            
            //if match() function returns more than 1 mateches, we create homography - essentiallty boundaries around the matched image.
            if(orbTracker.match()>1){
            //we create homography with detected img number. we do -1 as to access correct vectors for the images
                orbTracker.createHomography(orbTracker.getImgKeyPoints(orbTracker.getDetectedImgNumber()-1),orbTracker.getImgBoundaries(orbTracker.getDetectedImgNumber()-1));
                
                //we call firstDetect function in music player to change some variables
                musicMan.firstDetect();
                
            }
        }
        
        //once we have more than 3 keypoints to track we run flow tracker algorithm
        if(flow==true){
            //when we have 4 points in boundaries we pass them to optical flow function for easy tracking
            opticalFlow.updateFlowImage(vidGrabber.getPixels().getData(),orbTracker.getBoundariesKeyPoints(),orbMagic);
            //disabling detection/tracking/matching from orbTracker
            orbMagic = false;
            musicMan.play(true);

        }
    }
    
    //update music manager class with detected image number.
    musicMan.update(orbTracker.getDetectedImgNumber());
    
        //check gui buttons to control variables/states
        if(gui.detectButtonPressed()){
            orbMagic =!orbMagic;
            flow = false;
            orbTracker.reset();
            musicMan.play(false);
            gui.setbDetectButton();
        }
        
        //sound control previous song button check
        if(gui.prevButtonPressed()){
            musicMan.prevSong();
            gui.setbPrevButton();
        }
        //sound control next song button check
        if(gui.nextButtonPressed()){
            musicMan.nextSong();
            gui.setbNextButton();
        }

    //update visuals manger with the position of point where we can draw visuals, as well as state of music manager.
    vizMan.update(musicMan.isPlay(),opticalFlow.getMiddlePoint().x,opticalFlow.getMiddlePoint().y, 0, 10);
        
        //if music starts playing, pass along extraced sound feature values to visuals manager class
        if(musicMan.isPlay()){
            
            vizMan.setRMS(musicMan.getRms());
            vizMan.setSpecFlatness(musicMan.getSpecFlatness());
            vizMan.setSpecCentroid(musicMan.getSpecCentroid1());
            vizMan.setPeakFreq(musicMan.getPeakFreq());
            vizMan.setIsBeat(musicMan.getIsBeatDetected());
            
            //fft size/2
            for(int i=0; i < musicMan.getFftSize() / 2; i++) {
                vizMan.setFFTmagnitudes(musicMan.getFftMagnitudes(i));
            }
            
            //13 mffcs
            for(int i=0; i < 13; i++) {
                vizMan.setMFFCs(musicMan.getMfccs(i));
            }
            
            for(int i=0; i < musicMan.getOctaveAvgN(); i++) {
                vizMan.setOctaveAverages(musicMan.getOctaveAvg(i));
            }
            
            //devide pitches into 12
            for(int i=0; i < 12; i++) {
                vizMan.setPitchHistogram(musicMan.getPitchHistogram(i));
            }
            
            //42 mel bands
            for(int i=0; i < 42; i++) {
                vizMan.setMelBands(musicMan.getMelBands(i));
            }

        }
    }
}

    


//--------------------------------------------------------------
//Main draw function of the application, this is where we draw landing screen, gui, and audio visualisations
void arApp::draw(){
    //appstate0 = landing screen when the app is first started.
    if(appState==0){
        gui.drawStartingScreen();
    }
    
    //appstate1 = main part of the programm
    if(appState==1){
    
    //iPhone se size 568 x 320, camera img size 640 x 480
    //therefore we can center camera image with a small area being outside of visible screen.
    ofPushMatrix();
    ofTranslate(-80,-36);

    ofSetColor(255);
    vidGrabber.draw(0,0);
        
    //[debug] draw various features from orbTracker
    //orbTracker.draw();

    //we use this function to draw square around the perimeter of the screen when
    //the user presses detect button so that they would know what is going on.
    gui.drawDetectState(orbMagic);

    //here we draw the audio visuals if we get to the optical 'flow' state.
    if(flow){
        //first we check if tracking points from the image are visible
        if(opticalFlow.trackingPointsVisible()){
            
            //[debug]draw optical flow homography
            opticalFlow.drawHomography();
            
            //then we check if music manager plays the music,
            //if so we draw the visuals using fbo coming from visuals manager.
            if(musicMan.isPlay()){
                ofPushMatrix();
                ofSetColor(255);
                vizMan.drawFbo();
                ofSetColor(255);
                ofPopMatrix();
            }
        }else{
            //else if optical flow tracking points are out of sight, we stop playing the music.
            musicMan.play(false);
        }
    }
    //closing popmatrix after centering out video on the screen
    ofPopMatrix();
        
        //drawing gui, while checking if the music is playing. if the boolean is true,
        //we draw music control buttons and name of the album and music.
        gui.draw(musicMan.isPlay(),musicMan.getCurrentSongName());
        //we use this function to alert the user when the tracking points of the image are getting out of the sight
        gui.drawBoundariesWarning(opticalFlow.getWarning());
        //this is needed due to transparecy issues
        ofSetColor(255,255);
    }
}

//--------------------------------------------------------------
void arApp::exit(){

}

//--------------------------------------------------------------
void arApp::touchDown(ofTouchEventArgs & touch){
    
    //appState1 = main screen where we display camera feed and audio visuals
    if(appState==1){
        //checking locations of touch on the screen for the middle detect button
        gui.checkDetectButton(touch);

        //check music control buttons only when tracking points are visible
        if(opticalFlow.trackingPointsVisible()){
            //checks if the music control buttons have been pressed
            gui.checkMusicControls(touch);
        }
    }
}

//--------------------------------------------------------------
void arApp::touchMoved(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void arApp::touchUp(ofTouchEventArgs & touch){
    //appState0 = greeting menu screen
    //appState1 = main screen where we display camera feed and audio visuals
    //when we tap it goes to the main screen
    if(appState==0){
        appState=1;
    }
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
    
    //if we are in the main appstate and we detect image, play the audio out.
    if(appState==1){
        
        if(musicMan.isPlay()){
        //audio output comming from music manager class.
        musicMan.audioOut(output,bufferSize,nChannels);

        }else{
            for (int i = 0; i < bufferSize; i++){
                output[i] = 0.0;
            }
        }
    }
    
}




