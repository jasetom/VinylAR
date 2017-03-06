//
//  OrbTracker.hpp
//  VinylAR2
//
//  Created by Tomas on 16/02/2017.
//
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
    void createBoundaries();

    // detect, create and save keypoints and descriptors in the image
    void analyseImage(ofImage& img);
    
    // detect features in the upcomming stream of video pixels
    void detect(unsigned char * pix, int inputWidth, int inputHeight);
    void detect(Mat grayScaleVideo);

    
    // match current scene with analysed images
    int match(Mat descriptors);
    
    // calculate the perspective transform and apply to bounds polygon
    void createHomography(vector<KeyPoint> keyPoints, vector <Point2f> bounds);
    
    // access the tracked object:

    
    ofxCvGrayscaleImage trackImg;

    
    // Drawing options
    bool bDrawImage;
    bool bDrawFeatures;
    bool bDrawDescriptors;
    bool bDrawHomography;
    
    /*              Getters and setters         */
    // get the number of approved matches, a measure for the quality
    int getGoodMatchesSize();
    vector <KeyPoint> getImgKeyPoints();
    vector <Point2f> getImgBoundaries();
    Mat getImgDescriptors();
    
    //optical flow implementation
    vector <KeyPoint> getCameraKeyPoints();
    vector <Point2f> getGoodMatchesAsKeyPoints();
    vector <Point2f> getBoundariesKeyPoints();

    
    
private:
    float minMatches;
    ofxCvColorImage inputImg;
    ofxCvColorImage croppedImg;
    
    //tom
    ofxCvColorImage         analyseImg;
    ofxCvGrayscaleImage     greyImg;
    
    // keypoints and descriptors
    vector<KeyPoint> imgKeyPoints;			// keypoints of the object we're tracking
    Mat imgDescriptors;						// descriptors the object we're tracking
    vector <Point2f> imgBoundaries;				// bounds of the original object
    
    
    vector<KeyPoint> cameraKeyPoints;			// keypoints in the current scene
    Mat cameraDescriptors;						// descriptors in the current scene
    
    //	vector< DMatch > good_Matches;				// matches between original and new descriptors
    Mat homography;								// prespective transform between original and new features
    vector <Point2f> imgBoundariesTransformed;	// perspective transformed bounds
    
    OrbFeatureDetector detector;
    OrbDescriptorExtractor extractor;

    int nFeatures;
    
    BFMatcher matcher; //brute force matcher
    
    vector<DMatch> goodMatches;				// matches between original and new descriptors

    //optical flow implementation
    vector<Point2f> goodMatchesAsKeyPoints;
    
    
};


#endif /* OrbTracker_hpp */
