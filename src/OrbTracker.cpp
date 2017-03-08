//
//  OrbTracker.cpp
//  VinylAR2
//
//  Created by Tomas on 16/02/2017.
//
//  Some of the code is adapted from ofxSURFTRacker addon
//
//  Also from here http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//

#include "OrbTracker.hpp"


OrbTracker::OrbTracker(){
    
    //some boolean values initialisation
    
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
    
    //http://docs.opencv.org/2.4/modules/features2d/doc/feature_detection_and_description.html#orb-orb
    //    OrbFeatureDetector(
    //    int nfeatures=500,
    //    float scaleFactor=1.2f,
    //    int nlevels=8,
    //    int edgeThreshold=31,
    //    int firstLevel=0,
    //    int WTA_K=2,
    //    int scoreType=ORB::HARRIS_SCORE/ORB::FAST_SCORE
    //    int patchSize=31)
    
    
    matcher = BFMatcher(NORM_HAMMING, false);
    //    NORM_INF
    //    NORM_L1
    //    NORM_L2
    //    NORM_L2SQR
    //    NORM_HAMMING
    //    NORM_HAMMING2
    
}

//reference to constructor
OrbTracker::~OrbTracker(){
}


void OrbTracker::draw(){
    
    ofPushStyle();
    
    if(bDrawHomography){
        drawHomoGraphy();
    }
    if(bDrawImage){
        trackImg.draw(0,0);
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
void OrbTracker::detect(unsigned char *pix, int inputWidth, int inputHeight){
    //checks screen for features and descriptors
    
    // clear existing keypoints from previous frame
    cameraKeyPoints.clear();
    
    if(inputWidth != inputImg.getWidth() || inputHeight != inputImg.getHeight()){
        // this should only happen once
        inputImg.clear();
        inputImg.allocate(inputWidth, inputHeight);
        cout << "Re-allocated the input image."<<endl;
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
    
    /***
     code adapted from http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
     ***/
    
    // clear existing keypoints from previous frame
    cameraKeyPoints.clear();
    
    detector.detect(grayScaleVideo, cameraKeyPoints);
    
    // Calculate descriptors (feature vectors)
    extractor.compute(grayScaleVideo, cameraKeyPoints, cameraDescriptors);
    
}



void OrbTracker::createBoundaries(){
    
    // create bounding box to demonstrate the perspective transformation
    ofRectangle boundaries;
    imgBoundaries.clear();
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
           // matcher.knnMatch(descriptors,cameraDescriptors,matches,200);
            matcher.knnMatch(cameraDescriptors, matches, 200);

            
            //        flannMatcher.knnMatch(descriptors, descriptors_Scene, matches, 20);
            //        knnMatch( const Mat& queryDescriptors, const Mat& trainDescriptors,
            //                 CV_OUT vector<vector<DMatch> >& matches, int k,
            //                 const Mat& mask=Mat(), bool compactResult=false ) const;
        }
    
        goodMatches.clear();


        float ratio = 0.5;
        for(int i=0; i<matches.size(); i++){
            
            if((matches[i].size()==1)||(matches[i][0].distance/matches[i][1].distance<ratio)){
                goodMatches.push_back(matches[i][0]);
            }
        }


    
        if(goodMatches.size() > minMatches){
            // being here means we have found a decent match
            detectedImgNumber = goodMatches[minMatches].imgIdx;
            return goodMatches.size();
        }else{
            return 0;
        }

}

Mat OrbTracker::getImgDescriptors(int n){
    return manyImgDescriptors[n];
}



int OrbTracker::match2(){
    
    
    // Matching descriptor vectors using knn BF matcher
    vector< vector< DMatch > > matches;
    if(!cameraDescriptors.empty() ){

        //        matcher.knnMatch(cameraDescriptors, matches, 200);
        
        matcher.knnMatch(getImgDescriptors(detectedImgNumber),cameraDescriptors, matches, 200);

    }
    
    goodMatches.clear();
    
    float ratio = 0.5;
    for(int i=0; i<matches.size(); i++){
        if((matches[i].size()==1)||(matches[i][0].distance/matches[i][1].distance<ratio)){
            goodMatches.push_back(matches[i][0]);
        }
    }

    if(goodMatches.size() > minMatches){
        // being here means we have found a decent match
        return goodMatches.size();
    }else{
        return 0;
    }


}




//-----------------------------------------------------
void OrbTracker::createHomography(vector<KeyPoint> keyPoints, vector <Point2f> boundaries){
    
    //                orbTracker.createHomography(orbTracker.getImgKeyPoints(),orbTracker.getImgBoundaries());

//    
//    cout << "we are in create homography" << endl;
//
//    for(int i=0; i<keyPoints.size(); ++i){
//        cout <<"keypoints: " << keyPoints[i].size << endl;
//        
//    }
//    
//    for(int i=0; i<boundaries.size(); ++i){
//        cout <<"boundaries: " << boundaries[i] << endl;
//        
//    }
    
    
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
            imgPts.push_back(keyPoints[ goodMatches[i].queryIdx].pt);
            cameraPts.push_back(cameraKeyPoints[goodMatches[i].trainIdx].pt);
        }
        if( cameraPts.size() >5 && imgPts.size() > 5){
            homography = findHomography(imgPts,cameraPts,CV_RANSAC);
            perspectiveTransform(boundaries,imgBoundariesTransformed,homography);
        }
        
    }
    
}

//-----------------------------------------------------
void OrbTracker::analyseImage(ofImage &img){
    
    int inputWidth = img.getWidth();
    int inputHeight = img.getHeight();
    unsigned char *pix = img.getPixels().getData();

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
    
}

//-----------------------------------------------------
void OrbTracker::trainMatches(vector <cv::Mat> manyDescriptors){
    matcher.add(manyDescriptors);
    matcher.train();
    
}

//-----------------------------------------------------
//              Getters and setters
//-----------------------------------------------------

int OrbTracker::getGoodMatchesSize(){
    return goodMatches.size();
}

//-----------------------------------------------------
vector <KeyPoint> OrbTracker::getImgKeyPoints(int n){
           return manyImgKeyPoints[n];
//    return imgKeyPoints;
}

//-----------------------------------------------------
vector <Point2f> OrbTracker::getImgBoundaries(int n){
    return manyImgBoundaries[n];
//    return imgBoundaries;
}

//-----------------------------------------------------
Mat OrbTracker::getImgDescriptors(){
    return imgDescriptors;
}
//-----------------------------------------------------
vector <cv::Mat> OrbTracker::getManyImgDescriptors(){
    return manyImgDescriptors;
}

//-----------------------------------------------------
//implementing optical flow
vector <KeyPoint> OrbTracker::getCameraKeyPoints(){
    return cameraKeyPoints;
}

//-----------------------------------------------------
vector <Point2f> OrbTracker::getGoodMatchesAsKeyPoints(){
    
    if(!goodMatches.empty()){
        for(int i = 0; i < goodMatches.size();i++){
            Point2f keyPoint = cameraKeyPoints[goodMatches[i].queryIdx].pt;
            //            //            Point2f keyPoint = cameraKeyPoints[goodMatches[i].queryIdx].pt;
            goodMatchesAsKeyPoints.push_back(keyPoint);
            
        }
        return goodMatchesAsKeyPoints;
    }else{
        //return empty
        return goodMatchesAsKeyPoints;
    }
    
    
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

void OrbTracker::reset(){
    
    goodMatchesAsKeyPoints.clear();
    imgBoundariesTransformed.clear();
    goodMatches.clear();

    
    
}


