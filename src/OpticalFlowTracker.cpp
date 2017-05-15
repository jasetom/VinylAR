//
//  OpticalFlowTracker.cpp
//  VinylARt
//  Created by Tomas on 22/02/2017.
//
// This class was built using resources found in this forum post:
// https://forum.openframeworks.cc/t/cant-seem-to-get-takashis-opticalflowbm-method-to-work/6904/7
//

#include "OpticalFlowTracker.hpp"

//-----------------------------------------------------
// Setup opticalFlow tracker
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
    sideNo=0;

    
}

//-----------------------------------------------------
// This function creates and prepares grayscale image from camera pixels, which would be later on used to track keypoints
void OpticalFlowTracker::updateFlowImage(unsigned char* pix, vector<cv::Point2f> keyPointsToTrack, bool once){

    //we create image out of pixels comming from the camera
    currentImgColor.setFromPixels(pix, inputWidth, inputHeight);
    //make it grayscale
    currentImgGray = currentImgColor;
    //apply some effects, so it would be easier to track
    currentImgGray.blurGaussian(11);
    currentImgGray.threshold(100);
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
// before, we would use those instead and pass them onto another function.

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
/*This function is an implementation of calcOpticalFlowPyrLK() function provided by openCV library.

 Optical flow works by detecting movement in the pixels, and it allows to predict the movement or 'the flow'
 in the next frame of the video. The function to do this requires a whopping number of parameters - 13:
 
 bgImgGrayMat - previous frame pixels in grayscale
 currentImgGrayMat - current frame pixels in grayscale
 points_keyPoints - current frame keypoints
 points_nextPoints - predicted 'flow' keypoints for the next frame.
 status - returns status of the opticalflow algorithm
 err - returns error if something goes wrong with the optical flow algorithm
 winSize - window size in which optical flow happens
 maxLevel - maximal pyramid level number(defaults)
 termcrit - termination criteria of the iterative search algorithm (used default so not too sure what it does)
 flags - operational flags (used default one so not too sure how it works)
 
 */
void OpticalFlowTracker::trackFlowKeyPoints(vector<cv::Point2f> keyPoints){
    
    if(keyPoints.size()>=4){
        //explained above
        cv::calcOpticalFlowPyrLK(bgImgGrayMat,currentImgGrayMat,points_keyPoints,points_nextPoints,status, err, winSize, maxLevel, termcrit, flag);
        //once we are done with tracking for this frame, set the current points, and images to be the previous and run the whole thing again.
        bgImgGray = currentImgGray;
        points_keyPoints = points_nextPoints;
        
        //pass points to calculate middle point in our tracker
        calcMidPoint(points_nextPoints);
    }
}

//-----------------------------------------------------
void OpticalFlowTracker::drawHomography(int type){
    //this is where we draw square/line around the detected image.
    //currently quite buggy
    
    if(!points_nextPoints.empty()){
        
        //if type1 we draw square
        if(type==1){
        ofBeginShape();
        ofSetColor(233,233,233,100);
            
        ofSetLineWidth(10);
        
            //for each of the points derived from optical flow, we add a vertec position and draw a square shape
        for(int i=0; i < points_nextPoints.size(); i++){
            ofVertex(points_nextPoints[i].x,points_nextPoints[i].y);
        }
        
        if(points_nextPoints.size()>0){
            ofVertex(points_nextPoints[0].x, points_nextPoints[0].y);
        }
        
        ofEndShape();
            //if type2 we draw circles on each of the four points of the homography
    }else if(type==2){
        for( int i=0; i < points_nextPoints.size(); i++ ) {
            ofDrawCircle(points_nextPoints[i].x,points_nextPoints[i].y,5);
        }
    }

    }
}

//-----------------------------------------------------
//this function is used to calculated mid point on the detected image, so that we could draw visuals at that place
void OpticalFlowTracker::calcMidPoint(vector<cv::Point2f> boundariesPoints){
    //we take boundaries points and calculate center point,
    for(int i=0; i<boundariesPoints.size(); i++){
        cntrX = (boundariesPoints[0].x+boundariesPoints[2].x)/2;
        cntrY = (boundariesPoints[0].y+boundariesPoints[2].y)/2;
        // as well as scale which could be used later on in drawing
        scale = ofDist(boundariesPoints[0].x, boundariesPoints[0].y, boundariesPoints[2].x, boundariesPoints[2].y)/2;
        middlePoint.set(cntrX,cntrY);
    }
}

//-----------------------------------------------------
//this function checks if the tracking points are within the camera sight
//if any of them get out of the reach, we return false
bool OpticalFlowTracker::trackingPointsVisible(){
    
    sideNo =0;

    //as our camera width and height is bigger than the screen,
    //we can track a few pixels above the screen width and height
    //we set the side number to specific one in order to knwo where to draw the warning
    for(int i=0; i<points_nextPoints.size(); i++){
        //side1
        if(points_nextPoints[i].x < 65){
            sideNo=1;
        }
        //side2
        if(points_nextPoints[i].x > ofGetWidth()+100){
            sideNo=2;
        }
        //side3
        if(points_nextPoints[i].y < 10){
            sideNo=3;
        }
        //side4
        if(points_nextPoints[i].y > ofGetHeight()+95){
            sideNo=4;
        }
            
        //if any of the points are outside the view, we return false
        if(points_nextPoints[i].x < 40 || points_nextPoints[i].x > ofGetWidth()+150 ||
        points_nextPoints[i].y < 0 || points_nextPoints[i].y > ofGetHeight()+110){
            return false;
        }
    }
    return true;
}

//  Getter functions
//  these are used to interact with other classes
//-----------------------------------------------------
int OpticalFlowTracker::getWarning(){
    return sideNo;
}

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




