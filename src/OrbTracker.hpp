//
//  OrbTracker.hpp
//  VinylAR2
//
//  Created by Tomas on 16/02/2017.
//

#ifndef OrbTracker_hpp
#define OrbTracker_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxOpenCv.h"

// This depends on ofxOpenCV and opencv2
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;

class OrbTracker {
public:
    
    OrbTracker();
    ~OrbTracker();
    
    void draw();
    void drawFeatures();
    void drawDescriptors();
    void drawHomoGraphy();
    // detect, create and save keypoints and descriptors in the image
    void analyseImage(ofImage& img);
    void createBoundaries(vector<KeyPoint> imgKeyPoints);
    // detect features in the upcomming stream of video pixels
    void detect(unsigned char * pix, int inputWidth, int inputHeight);
    void detect(Mat grayScaleVideo);
    void trainMatches(vector <cv::Mat> manyDescriptors);
    void reset();
    // match current scene with analysed images
    int match();
    // calculate the perspective transform and apply to bounds polygon
    void createHomography(vector<KeyPoint> keyPoints, vector <Point2f> bounds);
    
    /*Getters*/
    vector <KeyPoint> getImgKeyPoints(int n);
    vector <Point2f> getImgBoundaries(int n);
    vector <KeyPoint> getCameraKeyPoints();
    vector <Point2f> getGoodMatchesAsKeyPoints();
    vector <Point2f> getBoundariesKeyPoints();
    vector <cv::Mat> getManyImgDescriptors();
    
    Mat getImgDescriptors(int n);
    int getDetectedImgNumber();
    int getGoodMatchesSize();
    
private:
    float minMatches;
    int detectedImgNumber;
    int nFeatures;
    // Drawing options
    bool bDrawImage;
    bool bDrawFeatures;
    bool bDrawDescriptors;
    bool bDrawHomography;
    
    vector<KeyPoint> cameraKeyPoints;			// keypoints in the current scene
    Mat cameraDescriptors;						// descriptors in the current scene
    Mat homography;								// prespective transform between original and new features
    
    vector <Point2f> imgBoundariesTransformed;	// perspective transformed bounds
    vector <vector <Point2f>> manyImgBoundaries;
    vector <vector <Point2f>> manyimgBoundariesTransformed;
    vector <vector <KeyPoint>> manyImgKeyPoints;
    vector <cv::Mat> manyImgDescriptors;
    
    OrbFeatureDetector detector;
    OrbDescriptorExtractor extractor;
    BFMatcher matcher; //brute force matcher
    
    vector<DMatch> goodMatches;	// matches between original and new descriptors
    vector<Point2f> goodMatchesAsKeyPoints;
    
};

#endif /* OrbTracker_hpp */
