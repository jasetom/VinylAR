//
//  OpticalFlowTracker.hpp
//  VinylARt
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
    //function declarations
    void setup(int cam_w, int cam_h);
    void updateFlowImage(unsigned char *pix, vector<cv::Point2f> keyPointsToTrack,bool once);
    void chooseFlowKeyPoints(vector<cv::Point2f> keyPointsToTrack,bool once);
    void trackFlowKeyPoints(vector<cv::Point2f> keyPointsToTrack);
    void drawHomography(int type);
    void calcMidPoint(vector<cv::Point2f> boundariesPoints);
    void calcScale(vector<cv::Point2f> boundariesPoints);
    bool trackingPointsVisible();
    
    //getters
    int getWarning();
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
    //keypoints
    vector<cv::KeyPoint> keyPointsCurrentImgGray;
    vector<cv::KeyPoint> keyPointsNextBgImgGray;
    //openCV image variables
    ofxCvGrayscaleImage pastImgGray;
    ofxCvGrayscaleImage bgImgGray;
    ofxCvGrayscaleImage currentImgGray;
    ofxCvColorImage		currentImgColor;
    //keypoints for flow tracking
    vector<cv::Point2f> points_keyPoints;
    vector<cv::Point2f> points_nextPoints;
    //openCV mat objects for flow tracking
    cv::Mat bgImgGrayMat;
    cv::Mat currentImgGrayMat;
    //other variables
    int inputWidth;
    int inputHeight;
    bool once;
    int sideNo;
    //middle point coordinates
    int cntrX;
    int cntrY;
    float scale;
    ofVec2f middlePoint;
};

#endif /* OpticalFlowTracker_hpp */
