//
//  OrbTracker.cpp
//  VinylAR2
//
//  Created by Tomas on 16/02/2017.
//
//  Code in this class is adapted from ofxSURFTRacker addon:
//  https://github.com/timknapen/ofxSURFTracker
//
//  Other OpenCv implementations are adapted from following links:
//  http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//  http://docs.opencv.org/2.4/modules/features2d/doc/feature_detection_and_description.html#orb-orb
//


#include "OrbTracker.hpp"


OrbTracker::OrbTracker(){
    
    //boolean values initialisation for drawing (mostly debuging)
    bDrawImage = false;
    bDrawFeatures = false;
    bDrawDescriptors = false;
    bDrawHomography = false;
    
    //number of features
    nFeatures = 500;
    //minumum matches
    minMatches = 13;
    
    //creating detector and matcher objs
    detector = OrbFeatureDetector(nFeatures,1.2f,8,30,0,2,ORB::HARRIS_SCORE,30);
    matcher = BFMatcher(NORM_HAMMING, false);
}

void OrbTracker::setup(){
    
    //load up images, resize if needed and analyse them using analyseImage() function.
    //all this is done manually for 5 images with the code below: 
    
    ////OrbTracker 0 pebbles
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/1.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 1 deer
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/2.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 2 man
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/3.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 3 wall
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/4.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 4 traintracks
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/5.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 5 roof
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/6.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    ////OrbTracker 6 car
    //load up tracker image and analyse it
    markerImg.load(ofToDataPath("images/7.jpg"));
    markerImg.resize(markerImg.getWidth()/2,markerImg.getHeight()/2);
    //analyse image to get its features
    analyseImage(markerImg);
    
    //once we analyse all of the images we train our matcher with descriptors
    trainMatches(getManyImgDescriptors());

    
    
}

//-----------------------------------------------------
void OrbTracker::analyseImage(ofImage &img){
    
    int inputWidth = img.getWidth();
    int inputHeight = img.getHeight();
    unsigned char *pix = img.getPixels().getData();
    
    ofxCvColorImage analyseImg;
    ofxCvGrayscaleImage greyImg;
    vector<KeyPoint> imgKeyPoints;// keypoints of the object we're analysing
    cv::Mat imgDescriptors; // descriptors the object we're analysing
    
    analyseImg.allocate(inputWidth, inputHeight);
    greyImg.allocate(inputWidth, inputHeight);
    
    // create the cvImage from the ofImage
    analyseImg.setFromPixels(pix, inputWidth, inputHeight);
    
    //set analysing image to grayscale
    greyImg = analyseImg;
    
    // get the mat to do the feature detection on grey image
    Mat imgMat = cvarrToMat(greyImg.getCvImage());
    //use detect function to detect and pass trackMat array to keypoints object
    detector.detect(imgMat, imgKeyPoints);
    
    manyImgKeyPoints.push_back(imgKeyPoints);
    
    // Calculate descriptors (feature vectors)
    extractor.compute(imgMat, imgKeyPoints, imgDescriptors);
    
    manyImgDescriptors.push_back(imgDescriptors);
    
    //analyse the image and create boundaries from its keypoints
    createBoundaries(imgKeyPoints);
}

//-----------------------------------------------------
void OrbTracker::trainMatches(vector <cv::Mat> manyDescriptors){
    matcher.add(manyDescriptors);
    matcher.train();
}

//-----------------------------------------------------
void OrbTracker::detect(unsigned char *pix, int inputWidth, int inputHeight){
    //checks screen for features and descriptors
    
    ofxCvColorImage inputImg;
    ofxCvGrayscaleImage trackImg;
    
    trackImg.allocate(inputWidth, inputHeight);
    
    // clear existing keypoints from previous frame
    cameraKeyPoints.clear();
    
    if(inputWidth != inputImg.getWidth() || inputHeight != inputImg.getHeight()){
        // this should only happen once
        inputImg.clear();
        inputImg.allocate(inputWidth, inputHeight);
    }
    
    // create the cvImage from the ofImage
    inputImg.setFromPixels(pix, inputWidth, inputHeight);
    // make it into a trackable grayscale image
    trackImg = inputImg;
    // get the Mat to do the feature detection on
    Mat imgMat = cvarrToMat(trackImg.getCvImage());
    detector.detect(imgMat, cameraKeyPoints);
    
    // Calculate descriptors (feature vectors)
    extractor.compute(imgMat, cameraKeyPoints, cameraDescriptors);
}

//maybe implement this in the future instead
//-----------------------------------------------------
void OrbTracker::detect(Mat grayScaleVideo){
    //checks screen for features and descriptors
    
    // clear existing keypoints from previous frame
    cameraKeyPoints.clear();
    
    detector.detect(grayScaleVideo, cameraKeyPoints);
    
    // Calculate descriptors (feature vectors)
    extractor.compute(grayScaleVideo, cameraKeyPoints, cameraDescriptors);
}

//-----------------------------------------------------
void OrbTracker::createBoundaries(vector<KeyPoint> imgKeyPoints){
    // create bounding box to demonstrate the perspective transformation
    ofRectangle boundaries;
    // bounds of the original object
    vector <Point2f> imgBoundaries;
    
    for(int i = 0; i < imgKeyPoints.size(); i++){
        Point2f pt = imgKeyPoints[i].pt;
        if(i == 0){
            boundaries.set(pt.x, pt.y, 0, 0);
        }
        boundaries.growToInclude(pt.x, pt.y);
    }
    
    Point2f p1(boundaries.position.x, boundaries.position.y);
    Point2f p2(boundaries.position.x + boundaries.width, boundaries.position.y + boundaries.height);
    imgBoundaries.push_back(p1);
    imgBoundaries.push_back(Point2f(p2.x, p1.y));
    imgBoundaries.push_back(Point2f(p2.x, p2.y));
    imgBoundaries.push_back(Point2f(p1.x, p2.y));
    
    //once we get the boundaries store them in vector for later use
    manyImgBoundaries.push_back(imgBoundaries);
}


//-----------------------------------------------------
int OrbTracker::match(){
    // this function tries to match keypoints and descriptors with the current scene
    // if there is no match, it returns 0,
    // if there is a match, it returns the number of matches
    
    // Matching descriptor vectors using knn BF matcher
    vector< vector< DMatch > > matches;
    if(!cameraDescriptors.empty() ){
        matcher.knnMatch(cameraDescriptors, matches, 20);

    }
    
    goodMatches.clear();
    
    float ratio = 0.5;
    for(int i=0; i<matches.size(); i++){
        
//        cout << matches[i].size() <<endl;
        
        if((matches[i].size()==1)||(matches[i][0].distance/matches[i][1].distance<ratio)){
            goodMatches.push_back(matches[i][0]);
        }
    }
    
    if(goodMatches.size() > minMatches){
        // being here means we have found a decent match, we add one so that if img number = 0 we assume that nothing is detected
        detectedImgNumber = goodMatches[minMatches].imgIdx+1;
        return goodMatches.size();
    }else{
        return 0;
    }
    
}

//-----------------------------------------------------
void OrbTracker::createHomography(vector<KeyPoint> keyPoints, vector <Point2f> boundaries){
    
    // saves the perspective transform and bounding box
    // find the homography
    // transform the bounding box for this scene
    vector <Point2f> cameraPts;
    vector <Point2f> imgPts;
    imgBoundariesTransformed.clear();
    
    if(goodMatches.size() > minMatches){
        for( int i = 0; i < goodMatches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            imgPts.push_back(keyPoints[goodMatches[i].trainIdx].pt);
            cameraPts.push_back(cameraKeyPoints[goodMatches[i].queryIdx].pt);
        }
        if( cameraPts.size() >5 && imgPts.size() > 5){
            
            
            
            homography = findHomography(imgPts,cameraPts,CV_RANSAC);
            perspectiveTransform(boundaries,imgBoundariesTransformed,homography);
        }
        
    }
    
}

//-----------------------------------------------------
void OrbTracker::draw(){
    
    //drawing was mostly used for debugging
    
    ofPushStyle();
    
    if(bDrawHomography){
        drawHomoGraphy();
    }
    
    if(bDrawDescriptors) {
        drawDescriptors();
    }
    
    if(bDrawFeatures){
        drawFeatures();
    }
    if(bDrawHomography){
        drawHomoGraphy();
    }
    
    ofPopStyle();
    
}



//-----------------------------------------------------
void OrbTracker::drawFeatures(){
    
    ofNoFill();
    ofSetColor(0, 255, 0);
    for(int i = 0; i < cameraKeyPoints.size(); i++){
        ofPushMatrix();
        ofTranslate(cameraKeyPoints[i].pt.x, cameraKeyPoints[i].pt.y);
        ofDrawLine(-2, -2, 2, 2);
        ofDrawLine(2, -2, -2, 2);
        ofPopMatrix();
        
    }
    
}

//-----------------------------------------------------
void OrbTracker::drawDescriptors(){
    ofNoFill();
    ofSetColor(0, 255, 0);
    for(int i = 0; i < cameraKeyPoints.size(); i++){
        KeyPoint kp = cameraKeyPoints[i];
        float l = kp.response/1000;
        ofDrawCircle(kp.pt.x, kp.pt.y,  l);
        ofDrawLine(kp.pt.x, kp.pt.y, kp.pt.x + l*cos(kp.angle), kp.pt.y + l*sin(kp.angle));
    }
}

//-----------------------------------------------------

void OrbTracker::drawHomoGraphy(){
    // Draw the transformed bounding box
    ofNoFill();
    ofBeginShape();
    ofSetLineWidth(5);
    
    int cntrX;
    int cntrY;
    int scale;
    
    //imgBoundariesTransformed = 4 points
    for(int i = 0; i < imgBoundariesTransformed.size(); i++){
        ofVertex(imgBoundariesTransformed[i].x, imgBoundariesTransformed[i].y);
        ofDrawBitmapString(i, imgBoundariesTransformed[i].x, imgBoundariesTransformed[i].y);
        
        cntrX = (imgBoundariesTransformed[0].x+imgBoundariesTransformed[2].x)/2;
        cntrY = (imgBoundariesTransformed[0].y+imgBoundariesTransformed[2].y)/2;
        scale = ofDist(imgBoundariesTransformed[0].x, imgBoundariesTransformed[0].y, imgBoundariesTransformed[2].x, imgBoundariesTransformed[2].y);
        
    }
    
    if(imgBoundariesTransformed.size() > 0){
        ofVertex(imgBoundariesTransformed[0].x, imgBoundariesTransformed[0].y);
        //draw sphere
        ofDrawSphere(cntrX,cntrY,200,scale/10);
    }
    
    ofEndShape();
}

//-----------------------------------------------------
void OrbTracker::reset(){
    goodMatchesAsKeyPoints.clear();
    imgBoundariesTransformed.clear();
    goodMatches.clear();
    
}

//-----------------------------------------------------
//              Getter functions
//-----------------------------------------------------
int OrbTracker::getGoodMatchesSize(){
    return goodMatches.size();
}

//-----------------------------------------------------
vector <KeyPoint> OrbTracker::getImgKeyPoints(int n){
    return manyImgKeyPoints[n];
}

//-----------------------------------------------------
vector <Point2f> OrbTracker::getImgBoundaries(int n){
    return manyImgBoundaries[n];
}

//-----------------------------------------------------
vector <cv::Mat> OrbTracker::getManyImgDescriptors(){
    return manyImgDescriptors;
}

//-----------------------------------------------------
vector <Point2f> OrbTracker::getBoundariesKeyPoints(){
    return imgBoundariesTransformed;
}

//-----------------------------------------------------
int OrbTracker::getDetectedImgNumber(){
    return detectedImgNumber;
}

//-----------------------------------------------------
Mat OrbTracker::getImgDescriptors(int n){
    return manyImgDescriptors[n];
}
