//
//  OpticalFlowTracker.hpp
//  opticalFlowAndTracking
//
//  Created by Tomas on 22/02/2017.
//

#ifndef OpticalFlowTracker_hpp
#define OpticalFlowTracker_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include <opencv2/video/tracking.hpp>

class OpticalFlowTracker{
    
public:
    
    void setup(int cam_w, int cam_h);
    void updateFlowImage(unsigned char *pix, vector<cv::Point2f> keyPointsToTrack);
    void chooseFlowKeyPoints(vector<cv::Point2f> keyPointsToTrack);
    void trackFlowKeyPoints(vector<cv::Point2f> keyPointsToTrack);
    void drawHomography();
    void calcMidPoint(vector<cv::Point2f> boundariesPoints);
    
    bool trackingPointsVisible();
    
    //getters
    ofVec2f getMiddlePoint();
    vector<cv::Point2f> getNextPoints();
    float getDrawingScalar();
    
private:
    //variables for calcOpticalFlowPyrLK function
    cv::TermCriteria termcrit;
    cv::vector <uchar> status;
    cv::vector <float> err;
    cv::Size winSize;
    int maxLevel;
    int flag;
    
    vector<cv::KeyPoint> keyPointsCurrentImgGray;
    vector<cv::KeyPoint> keyPointsNextBgImgGray;
    
    ofxCvGrayscaleImage pastImgGray;
    ofxCvGrayscaleImage bgImgGray;
    ofxCvGrayscaleImage currentImgGray;
    ofxCvColorImage		currentImgColor;
    
    vector<cv::Point2f> points_keyPoints;
    vector<cv::Point2f> points_nextPoints;
    
    cv::Mat bgImgGrayMat;
    cv::Mat currentImgGrayMat;
    
    int inputWidth;
    int inputHeight;
    
    bool once;
    
    //middle point coordinates
    int cntrX;
    int cntrY;
    float scale;
    ofVec2f middlePoint;
    
};

#endif /* OpticalFlowTracker_hpp */
