//
//  OpticalFlowTracker.hpp
//  VinylAR2
//
//  Created by Tomas on 01/03/2017.
//
//

#ifndef OpticalFlowTracker_hpp
#define OpticalFlowTracker_hpp

#include <stdio.h>

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <opencv2/video/tracking.hpp>
#include <stdio.h>


class OpticalFlowTracker{
    
public:
    OpticalFlowTracker();
    void setup(int cam_w, int cam_h);
    void updateFlowImage(unsigned char *pix, vector<cv::Point2f> keyPointsToTrack);
    void chooseFlowKeyPoints(cv::Mat currentMat,vector<cv::Point2f> keyPointsToTrack);
    void trackFlowKeyPoints(vector<cv::Point2f> keyPoints,cv::Mat currentMat);
    void drawHomography();
    
    int getState();
    vector<cv::Point2f> getNextPoints();
    
private:
    
    cv::TermCriteria termcrit;
    cv::vector <uchar> status;
    cv::vector <float> err;
    cv::Size winSize;
    
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
    int state;
    int maxLevel;
    bool on;
    
    
};
#endif /* OpticalFlowTracker_hpp */
