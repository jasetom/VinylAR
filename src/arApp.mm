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

    //basic setup for programm
    vidGrabber.initGrabber(640,480);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/marker1.jpg"));
    
    ////OrbTracker
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    //create boundaries for the analysed image
    orbTracker.createBoundaries();
    
    ////OpticalFlowTracker
    opticalFlow.setup(640, 480);
    
    ////Sound setup
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
    
    //left and right audio output sizes
    lAudio = new float[initialBufferSize];
    rAudio = new float[initialBufferSize];
    
    //loading sample track
    samp1.load(ofToDataPath("sounds/beat2.wav"));
    samp2.load(ofToDataPath("sounds/beat3.wav"));
    samp3.load(ofToDataPath("sounds/beat4.wav"));
    
    samp = samp1;
    
    //set up maxim
    ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
    
    //setup fft
    fftSize = 1024;
    mfft.setup(fftSize, 512, 256);
    
    //this allows to mix audio from other apps
    //    ofxiOSSoundStream::setMixWithOtherApps(true);
    //this starts DAC using opeframeworks
    ofSoundStreamSetup(2, 0, this, sampleRate, initialBufferSize, 4);

    
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
            //once detection has been run we match and create homography, which creates boundaries keypoints.
            if(orbTracker.match(orbTracker.getImgDescriptors(),1)>1){
                orbTracker.createHomography(orbTracker.getImgKeyPoints(),orbTracker.getImgBoundaries());
            }
        }
        
        if(flow==true){
            //when we have 4 points in boundaries we pass them to optical flow function for easy tracking
            opticalFlow.updateFlowImage(vidGrabber.getPixels().getData(),orbTracker.getBoundariesKeyPoints());
            //disabling detection/tracking/matching from orbTracker
            orbMagic = false;
            
        }
    
    }
    //check sound
    if(flow){
        playSound = true;
    }else{
        playSound = false;
    }
    
}

    


//--------------------------------------------------------------
void arApp::draw(){
    
    //iphone se size 1136 x 640
    //      img size 640 x 480
    ofSetColor(255);
    vidGrabber.draw(0,0);
    
    //draw op
    opticalFlow.drawHomography();
    
    //draw sound
    
    ofBeginShape();
    
    ofSetColor(255,133,133);
    ofFill();

    for(int i=0; i < fftSize / 8; i++) {
        
        ofSetColor(7*i,133,133);
        //place spheres in a circle using trig functions
        int x = cos(i)*100;
        int y = sin(i)*100;
        ofDrawSphere(x,y,150,5+mfft.magnitudes[i]*2);
    }
    
    ofSetColor(200);
    ofDrawRectangle(-100,0,0,200,30);
    ofSetColor(255,0,0);
    ofDrawBitmapString("Artist - SoundTrackName", -100, 0,10);

    ofEndShape();
    
   

    
}

//--------------------------------------------------------------
void arApp::exit(){

}

//--------------------------------------------------------------
void arApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void arApp::touchMoved(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void arApp::touchUp(ofTouchEventArgs & touch){
    orbMagic =!orbMagic;

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
    for (int i = 0; i < bufferSize; i++){
        //check wheter to play sound. Plays only when the AR marker is detected.
        if(playSound == true){
            
            //play sound
            playingSound = samp.play();
            
            //process sound and convert magnitudes to decibels in the mfft.
            if (mfft.process(playingSound)) {
                mfft.magsToDB();
            }
            //volume
            lAudio[i] = output[i * nChannels] = playingSound;// * 0.0;
            rAudio[i] = output[i * nChannels + 1] = playingSound;//* 0.0;
            
        }else{
            //if not playing set output to zero.
            lAudio[i] = output[i * nChannels] = 0;
            rAudio[i] = output[i * nChannels + 1] = 0;
        }
    }
}
