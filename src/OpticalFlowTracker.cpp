//
//  OpticalFlowTracker.cpp
//  VinylAR2
//
//  Created by Tomas on 01/03/2017.
//
//

#include "OpticalFlowTracker.hpp"


OpticalFlowTracker::OpticalFlowTracker(){
    
    //constructor
}

void OpticalFlowTracker::setup(int cam_w, int cam_h){
    
    inputWidth = cam_w;
    inputHeight = cam_h;
    
    currentImgColor.allocate(cam_w, cam_h);
    currentImgGray.allocate(cam_w, cam_h);
    bgImgGray.allocate(cam_w, cam_h);
    
    //OpticalFlowPyrLK parameters
    termcrit = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
    winSize = cvSize(20,20);
    maxLevel = 3;
    
    once = true;
    state = 1;
    
}

void OpticalFlowTracker::updateFlowImage(unsigned char *pix, vector<cv::Point2f> keyPointsToTrack){
    
    //we create image out of pixels comming from the camera
    currentImgColor.setFromPixels(pix, inputWidth, inputHeight);
    //make it grayscale
    currentImgGray = currentImgColor;
    //apply some effects, so it would be easier to track
    currentImgGray.blurGaussian(7);
    currentImgGray.threshold(75);
    currentImgGray.blurGaussian(1);
    
    //convert image to cv mat
    cv::Mat currentImgGrayMat = cv::cvarrToMat(currentImgGray.getCvImage());
    cv::Mat bgImgGrayMat = cv::cvarrToMat(bgImgGray.getCvImage());
    
    
    cout << "updateFlowImage" << endl;
    
    //pass along mat array to another function
    chooseFlowKeyPoints(currentImgGrayMat,keyPointsToTrack);
    
}


void OpticalFlowTracker::chooseFlowKeyPoints(cv::Mat currentMat, vector<cv::Point2f> keyPointsToTrack){
    //
    std::vector<cv::KeyPoint> keyPointsCurrentImgGray;
    std::vector<cv::KeyPoint> keyPointsNextBgImgGray;
    
    //    run this for the very first frame or if the keypoints are empty yet
    if(once==true && keyPointsToTrack.empty()){
        
        //            cv::FAST(currentMat, keyPointsCurrentImgGray,30,true);
        //            cv::KeyPoint::convert(keyPointsCurrentImgGray, points_keyPoints);
        
        //            bgImgGrayMat = cv::cvarrToMat(bgImgGray.getCvImage());
        cv::FAST(bgImgGrayMat, keyPointsNextBgImgGray,30,true);
        cv::KeyPoint::convert(keyPointsNextBgImgGray, points_nextPoints);
        once = false;
        
    }else{
        if(!keyPointsToTrack.empty()){
            state = 2;
            trackFlowKeyPoints(keyPointsToTrack,currentMat);
        }
        
    }
    
}


void OpticalFlowTracker::trackFlowKeyPoints(vector<cv::Point2f> keyPoints,cv::Mat currentMat){
    
    //if we get 4 keypoints of boundaries, use optic flow to track points.
    if(keyPoints.size()>=4){
        cout << "TrackFlowKeyPoints Points>4" << endl;
        
        //track points using opticalFlowLK
        cv::calcOpticalFlowPyrLK(bgImgGrayMat,currentMat,keyPoints,points_nextPoints,status, err, winSize, maxLevel, termcrit,0);
        
        //  bgImgGray = currentImgGray;
        //            bgImgGrayMat = currentMat;
        // keyPoints = points_nextPoints;
        //cout << " below TrackFlowKeyPoints Points" << endl;
        
    }
}

void OpticalFlowTracker::drawHomography(){
    
    int pointCount = points_nextPoints.size();
    
    ofDrawBitmapString("pointCount: " + ofToString(pointCount),20,20);
    
    //    ofBeginShape();
    //    ofSetColor(0,255,0);
    //    ofSetLineWidth(5);
    //    for( int i=0; i < points_nextPoints.size(); i++ ) {
    //        ofVertex(points_nextPoints[i].x,points_nextPoints[i].y);
    //        ofDrawBitmapString(i, points_nextPoints[i].x,points_nextPoints[i].y);
    //
    //    }
    //    if(points_nextPoints.size() > 0){
    //        ofVertex(points_nextPoints[0].x, points_nextPoints[0].y);
    //    }
    //
    //    ofEndShape();
    
    
}

int OpticalFlowTracker::getState(){
    return state;
}

vector<cv::Point2f> OpticalFlowTracker::getNextPoints(){
    return points_nextPoints;
}
