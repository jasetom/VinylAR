//
//  OpticalFlowTracker.cpp
//  opticalFlowAndTracking
//
//  Created by Tomas on 22/02/2017.
//
// This class was built using resources found in this forum post:
// https://forum.openframeworks.cc/t/cant-seem-to-get-takashis-opticalflowbm-method-to-work/6904/7
//

#include "OpticalFlowTracker.hpp"

//-----------------------------------------------------

void OpticalFlowTracker::setup(int cam_w, int cam_h){
    //size variables
    inputWidth = cam_w;
    inputHeight = cam_h;
    //allocating images required for optic flow tracking
    currentImgColor.allocate(cam_w, cam_h);
    currentImgGray.allocate(cam_w, cam_h);
    bgImgGray.allocate(cam_w, cam_h);
    //OpticalFlowPyrLK openCV function parameters
    termcrit = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
    winSize = cvSize(20,20);
    maxLevel = 3;
    flag = 0;
    //boolean which specifies to run certain parts of the code only once.
    once = true;
    
}

//-----------------------------------------------------
// This function creates and prepares grayscale image from camera pixels, which would be later on used to track keypoints

void OpticalFlowTracker::updateFlowImage(unsigned char* pix, vector<cv::Point2f> keyPointsToTrack, bool once){

    //we create image out of pixels comming from the camera
    currentImgColor.setFromPixels(pix, inputWidth, inputHeight);
    //make it grayscale
    currentImgGray = currentImgColor;
    //apply some effects, so it would be easier to track
    currentImgGray.blurGaussian(5);
    currentImgGray.threshold(75);
    currentImgGray.blurGaussian(1);
    
    //convert image to cv mat
    currentImgGrayMat = cv::cvarrToMat(currentImgGray.getCvImage());
    bgImgGrayMat = cv::cvarrToMat(bgImgGray.getCvImage());
    
    //when we are done with preparations, pass along keypoints to another function
    chooseFlowKeyPoints(keyPointsToTrack,once);
    
}

//-----------------------------------------------------
// This function is called at the end of updateFlowImage() function. It acts as a gate which
// chooses the correct keypoints to track. When the function gets called for the very first time, it will
// use cv::FAST extractor to get keyPoints from image. But once we've got keyPointsToTrack passed from the function
//before, we would use those instead and pass them onto another function.

void OpticalFlowTracker::chooseFlowKeyPoints(vector<cv::Point2f> keyPointsToTrack,bool once){
    
    //check if the function is called for the first time or keyPoints are empty
    if(once==true){
        //if so, we use FAST image detector to get keypoints to get going with our program
        cv::FAST(currentImgGrayMat, keyPointsCurrentImgGray,30,true);
        //then we convert openCV Keypoints to openCV Vector points
        cv::KeyPoint::convert(keyPointsCurrentImgGray, points_keyPoints);
        
        //we do the same with the previous image
        //bgImgGrayMat = cv::cvarrToMat(bgImgGray.getCvImage());
        cv::FAST(bgImgGrayMat, keyPointsNextBgImgGray,30,true);
        cv::KeyPoint::convert(keyPointsNextBgImgGray, points_nextPoints);
        //then we set keypoints to be the received keypoints
        points_keyPoints = keyPointsToTrack;
        //and use those points to track
       // trackFlowKeyPoints(points_keyPoints);
        once = false;
    }
        //pass them to the function for tracking
        trackFlowKeyPoints(points_keyPoints);

    
}

//-----------------------------------------------------
//This function is an implementation of calcOpticalFlowPyrLK() function provided by openCV library.
/*
 
 It requires a whopping number of parameters - 13! So let me explain:
 
 bgImgGrayMat -
 currentImgGrayMat -
 points_keyPoints -
 points_nextPoints -
 status -
 err -
 winSize -
 maxLevel -
 termcrit -
 flags -
 
 */

void OpticalFlowTracker::trackFlowKeyPoints(vector<cv::Point2f> keyPoints){
    
    if(keyPoints.size()>=4){
        cv::calcOpticalFlowPyrLK(bgImgGrayMat,currentImgGrayMat,points_keyPoints,points_nextPoints,status, err, winSize, maxLevel, termcrit, flag);
        //once we are done with tracking for this frame, set the current points, and images to be the previous and run the whole thing again.
        bgImgGray = currentImgGray;
        points_keyPoints = points_nextPoints;
        
        //pass points to calculate middle point in our tracker
        calcMidPoint(points_nextPoints);
    }
}

//-----------------------------------------------------

void OpticalFlowTracker::drawHomography(){
    
    if(!points_nextPoints.empty()){
        
        ofBeginShape();
        ofSetColor(0,255,0);
        ofSetLineWidth(5);
        ofNoFill();

        for( int i=0; i < points_nextPoints.size(); i++ ) {
            ofVertex(points_nextPoints[i].x,points_nextPoints[i].y);
            ofDrawBitmapString(i, points_nextPoints[i].x,points_nextPoints[i].y);
            
        }
        
        if(points_nextPoints.size() > 0){
            ofVertex(points_nextPoints[0].x, points_nextPoints[0].y);
        }
        
        ofEndShape();
    }
    

    
}

//-----------------------------------------------------

void OpticalFlowTracker::calcMidPoint(vector<cv::Point2f> boundariesPoints){
    //we take boundaries points and calculate center point, as well as scale which could be used later on in drawing.
    for(int i=0; i<boundariesPoints.size(); i++){
        cntrX = (boundariesPoints[0].x+boundariesPoints[2].x)/2;
        cntrY = (boundariesPoints[0].y+boundariesPoints[2].y)/2;
        scale = ofDist(boundariesPoints[0].x, boundariesPoints[0].y, boundariesPoints[2].x, boundariesPoints[2].y);
        middlePoint.set(cntrX,cntrY);
    }
}

//-----------------------------------------------------

bool OpticalFlowTracker::trackingPointsVisible(){
    
//as our camera width and height is bigger than the screen, we can track a few pixels above the screen width and height
        for(int i=0; i<points_nextPoints.size(); i++){
            
//            80,36,ofGetWidth(),ofGetHeight()
            
            if(points_nextPoints[i].x < 60 || points_nextPoints[i].x > ofGetWidth()+100 ||
            points_nextPoints[i].y < 16 || points_nextPoints[i].y > ofGetHeight()+100){
                return false;
            }

        }
    
    return true;
}



//  Getters
//-----------------------------------------------------

vector<cv::Point2f> OpticalFlowTracker::getNextPoints(){
    return points_nextPoints;
}

//-----------------------------------------------------
ofVec2f OpticalFlowTracker::getMiddlePoint(){
    return middlePoint;
}

//-----------------------------------------------------
float OpticalFlowTracker::getDrawingScalar(){
    return scale/10;
}




