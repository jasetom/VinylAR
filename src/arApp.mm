//
// Created by Tomas Jasevicius
//


#include "arApp.h"

//--------------------------------------------------------------
void arApp::setup(){
    
    /////Old OrbTracker implementation.
//    //Set orientation to portait
//    ofSetOrientation(OF_ORIENTATION_DEFAULT);
//    ofSetVerticalSync(true);
//    ofSetFrameRate(60);
//    //innitiate video grabber
//    vidGrabber.setDevice(0);
//    
//    vidGrabber.initGrabber(640,480);
//    
//    //load up tracker image and analyse it
//    markerImg.load(ofToDataPath("images/marker1.jpg"));
//    
//    //analyse image to get its features
//    orbTracker.analyseImage(markerImg);
//    orbTracker.createBoundaries();
//    ofSetFrameRate(30);
    ////

    
    
    //implementing opTracker down here
//    opTracker.setup(vidGrabber.getWidth(), vidGrabber.getHeight());
    
    
    
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    
    vidGrabber.initGrabber(640,480);
    
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/marker1.jpg"));
    
    //analyse image to get its features
    orbTracker.analyseImage(markerImg);
    orbTracker.createBoundaries();

    counter=0;
    
    currentImgColor.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    currentImgGray.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    //    pastImgGray.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    bgImgGray.allocate(vidGrabber.getWidth(), vidGrabber.getHeight());
    
    once = true;
    state = 1;
    

    
    
    
    


}

//--------------------------------------------------------------
void arApp::update(){
    
    vidGrabber.update();
    
    
    if(vidGrabber.isFrameNew()){
        
        currentImgColor.setFromPixels(vidGrabber.getPixels().getData(), vidGrabber.getWidth(), vidGrabber.getHeight());
        
        currentImgGray = currentImgColor;
        
        
        currentImgGray.blurGaussian(6);
        currentImgGray.threshold(75);
        currentImgGray.blurGaussian(1);
        
        std::vector<cv::KeyPoint> keyPointsCurrentImgGray;
        std::vector<cv::KeyPoint> keyPointsNextBgImgGray;
        
        //convert images to cv mats
        Mat currentImgGrayMat = cvarrToMat(currentImgGray.getCvImage());
        
        Mat bgImgGrayMat = cvarrToMat(bgImgGray.getCvImage());
        
        //now we detect and match using ORB class:
        orbTracker.detect(vidGrabber.getPixels().getData(),vidGrabber.getWidth(), vidGrabber.getHeight());
        
        //use matcher with state parameter and create homography when state is 1
        if(state ==1){
            orbTracker.match(orbTracker.getImgDescriptors(),state);
            orbTracker.createHomography(orbTracker.getImgKeyPoints(),orbTracker.getImgBoundaries());
        }
        
        //run this for the very first frame or if the keypoints are empty
        if(once==true||orbTracker.getBoundariesKeyPoints().empty()){
            keyPointsCurrentImgGray = orbTracker.getCameraKeyPoints();
            cv::KeyPoint::convert(keyPointsCurrentImgGray, points_keyPoints);
            
            bgImgGrayMat = cvarrToMat(bgImgGray.getCvImage());
            cv::FAST(bgImgGrayMat, keyPointsNextBgImgGray,30,true);
            cv::KeyPoint::convert(keyPointsNextBgImgGray, points_nextPoints);
            once = false;
        }else{
            if(!orbTracker.getBoundariesKeyPoints().empty() && state==1){
                points_keyPoints = orbTracker.getBoundariesKeyPoints();
                
                state = 2;
            }
        }
        
        //variables
        cv::TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
        cv::vector<uchar> status;
        cv::vector<float> err;
        cv::Size winSize(20,20);
        
        //if we get 4 keypoints of boundaries, use optic flow to track points.
        if(orbTracker.getBoundariesKeyPoints().size()>=4){
            //track points using opticalFlowLK
            cv::calcOpticalFlowPyrLK(bgImgGrayMat,currentImgGrayMat,points_keyPoints,points_nextPoints,status, err, winSize, 3, termcrit, 0);
            
            bgImgGray = currentImgGray;
            bgImgGrayMat = currentImgGrayMat;
            points_keyPoints = points_nextPoints;
        }
        
        
    }
    
    
}




    
    
    
    /////Old OrbTracker implementation.
////    //if the frame is new create new picture out of pixels
//    vidGrabber.update();
//    if(vidGrabber.isFrameNew()){
////        //create new video image
//        tempPix.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), vidGrabber.getPixelFormat());
//        videoImg.setFromPixels(tempPix);
//
//        orbTracker.detect(vidGrabber.getPixels(),vidGrabber.getWidth(), vidGrabber.getHeight());
//
//        if(bDetect){
//            if(orbTracker.match(orbTracker.getImgKeyPoints(),orbTracker.getImgDescriptors(),orbTracker.getImgBoundaries())>1){
//               orbTracker.createHomography(orbTracker.getImgKeyPoints(),orbTracker.getImgBoundaries());
//            }
//        }
//    }
//}
    ////

    


//--------------------------------------------------------------
void arApp::draw(){
    
    
    
    ofSetColor(255);
    currentImgColor.draw(0,0);
    ofSetColor(0xffffff);
    //    int pointCount = points_nextPoints.size();
    //    for( int i=0; i < points_nextPoints.size(); i++ ) {
    //        ofNoFill();
    //        ofSetHexColor(0xFF0000);
    //
    //        ofDrawSphere(points_nextPoints[i].x,points_nextPoints[i].y,2);
    //    }
    
    
    
    int pointCount = points_nextPoints.size();
    ofDrawBitmapString("pointCount: " + ofToString(pointCount),20,20);
    
    ofBeginShape();
    ofSetColor(0,255,0);
    ofSetLineWidth(5);
    for( int i=0; i < points_nextPoints.size(); i++ ) {
        ofVertex(points_nextPoints[i].x,points_nextPoints[i].y);
        ofDrawBitmapString(i, points_nextPoints[i].x,points_nextPoints[i].y);
        
        //        cntrX = (imgBoundariesTransformed[0].x+imgBoundariesTransformed[2].x)/2;
        //        cntrY = (imgBoundariesTransformed[0].y+imgBoundariesTransformed[2].y)/2;
        //        scale = ofDist(imgBoundariesTransformed[0].x, imgBoundariesTransformed[0].y, imgBoundariesTransformed[2].x, imgBoundariesTransformed[2].y);
        
    }
    if(points_nextPoints.size() > 0){
        ofVertex(points_nextPoints[0].x, points_nextPoints[0].y);
    }
    
    ofEndShape();
    
    
    

    
    
    /////Old OrbTracker implementation.
//    videoImg.draw(0,0);
//    orbTracker.draw();
    ////
    
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
