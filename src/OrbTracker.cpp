//
//  OrbTracker.cpp
//  VinylARt
//
//  Created by Tomas on 16/02/2017.
//
//  Code in this class is adapted and very heavily modfied from ofxSURFTRacker addon:
//  https://github.com/timknapen/ofxSURFTracker
//
//  OpenCv implementations are adapted from following links:
//  http://docs.opencv.org/doc/tutorials/features2d/feature_homography/feature_homography.html
//  http://docs.opencv.org/2.4/modules/features2d/doc/feature_detection_and_description.html#orb-orb
//

#include "OrbTracker.hpp"

OrbTracker::OrbTracker(){
    
    //boolean values initialisation for drawing (mostly debuging)
    bDrawImage = false;
    bDrawFeatures = true;
    bDrawDescriptors = false;
    bDrawHomography = false;

    //minumum matches
    minMatches = 13;

    //creating detector and matcher objs
    detector = OrbFeatureDetector();
    matcher = BFMatcher(NORM_HAMMING, false);
}

void OrbTracker::setup(){
    
    //load up images, resize if needed and analyse them using analyseImage() function.
    //all this is done manually for 5 images with the code below: 
    
    int numb = 4;
    
    ////OrbTracker 1 youngman w camera
    //load up tracker image and analyse it
    markerImg1.load(ofToDataPath("images/album1.jpg"));
    markerImg1.resize(markerImg1.getWidth()/numb,markerImg1.getHeight()/numb);
    //analyse image to get its features
    analyseImage(markerImg1);
    
    ////OrbTracker 2 pebbles
    //load up tracker image and analyse it
    markerImg2.load(ofToDataPath("images/album2.jpg"));
    markerImg2.resize(markerImg2.getWidth()/numb,markerImg2.getHeight()/numb);
    //analyse image to get its features
    analyseImage(markerImg2);
    
    ////OrbTracker 3 oldman
    //load up tracker image and analyse it
    markerImg3.load(ofToDataPath("images/album3.jpg"));
    markerImg3.resize(markerImg3.getWidth()/numb,markerImg3.getHeight()/numb);
    //analyse image to get its features
    analyseImage(markerImg3);
    
    


    
    //once we analyse all of the images we train our matcher with descriptors
    trainMatches(getManyImgDescriptors());
}

//-----------------------------------------------------
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
void OrbTracker::analyseImage(ofImage &img){
    
    //we take data from input image, such as heigh and width
    int inputWidth = img.getWidth();
    int inputHeight = img.getHeight();
    //also we take contents of the picture pixel by pixel
    unsigned char *pix = img.getPixels().getData();
    
    //create new openCV image objects
    ofxCvColorImage analyseImg;
    ofxCvGrayscaleImage greyImg;
    
    vector<KeyPoint> imgKeyPoints;// keypoints of the object we're analysing
    cv::Mat imgDescriptors; // descriptors the object we're analysing
    
    //allocate size for opencv images with our input image sizes
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
    
    //store all of the detected keypoints into manyImgKeypoints vector
    manyImgKeyPoints.push_back(imgKeyPoints);
    
    //calculate descriptors (feature vectors)
    extractor.compute(imgMat, imgKeyPoints, imgDescriptors);
    
    //store all of the detected descriptors into manyImgDecriptors vector
    manyImgDescriptors.push_back(imgDescriptors);
    
    //analyse the image and create boundaries from its keypoints
    createBoundaries(imgKeyPoints);
}

//-----------------------------------------------------
void OrbTracker::trainMatches(vector <cv::Mat> manyDescriptors){
    //this is how we train our matcher object. We simply pass manyDescriptors vector into the object
    //and algorithm provided by openCV stores the values for later matching.
    matcher.add(manyDescriptors);
    matcher.train();
}

//-----------------------------------------------------
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
//this function checks screen and detects features and descriptors
//works the same as analyse() function, except we use camera feed to do the analysis instead of still image
void OrbTracker::detect(unsigned char *pix, int inputWidth, int inputHeight){

    //create new openCV objects
    ofxCvColorImage inputImg;
    ofxCvGrayscaleImage trackImg;
    
    //allocate image sizes
    trackImg.allocate(inputWidth, inputHeight);
    
    //clear existing keypoints from previous frame
    cameraKeyPoints.clear();
    
    if(inputWidth != inputImg.getWidth() || inputHeight != inputImg.getHeight()){
        //this should only happen once
        inputImg.clear();
        inputImg.allocate(inputWidth, inputHeight);
    }
    
    //create the cvImage from the ofImage
    inputImg.setFromPixels(pix, inputWidth, inputHeight);
    //make it into a trackable grayscale image
    trackImg = inputImg;
    //get the Mat to do the feature detection on
    Mat imgMat = cvarrToMat(trackImg.getCvImage());
    //use detector object to detect features using keypoints from camera
    detector.detect(imgMat, cameraKeyPoints);
    
    //calculate descriptors (feature vectors) from cameraKeypoints
    extractor.compute(imgMat, cameraKeyPoints, cameraDescriptors);
}


//-----------------------------------------------------
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
//this function creates boundaries around the detected images keypoints
void OrbTracker::createBoundaries(vector<KeyPoint> imgKeyPoints){
    //create bounding box to demonstrate the perspective transformation
    ofRectangle boundaries;
    //boundaries of the original object
    vector <Point2f> imgBoundaries;
    //for each keypoint in the image, we put it into a rectangle.
    //if the current keypoint is outside the rectangle we use growToInclude function to make
    //the rectangle bigger and include all of the image keypoints.
    for(int i = 0; i < imgKeyPoints.size(); i++){
        Point2f pt = imgKeyPoints[i].pt;
        if(i == 0){
            boundaries.set(pt.x, pt.y, 0, 0);
        }
        boundaries.growToInclude(pt.x, pt.y);
    }
    //here we only extract the 4 corner points of the rectangle
    Point2f p1(boundaries.position.x, boundaries.position.y);
    Point2f p2(boundaries.position.x + boundaries.width, boundaries.position.y + boundaries.height);
    //and push them to imgBoundaries vector
    imgBoundaries.push_back(p1);
    imgBoundaries.push_back(Point2f(p2.x, p1.y));
    imgBoundaries.push_back(Point2f(p2.x, p2.y));
    imgBoundaries.push_back(Point2f(p1.x, p2.y));
    
    //once we get the boundaries of the single image,
    //store them in vector with multiple images for later use
    manyImgBoundaries.push_back(imgBoundaries);
}


//-----------------------------------------------------
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
int OrbTracker::match(){
    // this function tries to match keypoints and descriptors with the current scene
    // if there is no match, it returns 0,
    // if there is a match, it returns the number of matches
    
    // Matching descriptor vectors using knn BF matcher
    vector< vector< DMatch > > matches;
    if(!cameraDescriptors.empty()){
        matcher.knnMatch(cameraDescriptors, matches, 20);

    }
    //clear goodMatches vector before filling it up
    goodMatches.clear();
    
    //this is how we sort the matches. We check the distance between the distance of the analysed image features
    //and camera feed features. If the distance is less than manually set 0.7 ratio when we devide features matched,
    //we consider a match to be a good match.
    float ratio = 0.7;
    for(int i=0; i<matches.size(); i++){
        if((matches[i].size()==1)||(matches[i][0].distance/matches[i][1].distance<ratio)){
            //if match is good, we store it in the vector
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
//this function creates homography, which is basiclly grouping of keypoints.
//it transforms the boundaries box for the object detected.
//it is used to draw a square around the object matched.
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
void OrbTracker::createHomography(vector<KeyPoint> keyPoints, vector <Point2f> boundaries){
    //create two vectors for camera points and image points
    vector <Point2f> cameraPts;
    vector <Point2f> imgPts;
    //clear the boundaries before creating new ones
    imgBoundariesTransformed.clear();
    
    //if we have more good matches than minMatches(which is manually set),
    //we populate imgPts and cameraPts vectors with keypoints from earlier.
    //however, we only take specific points from both camera, and analysed keypoints.
    //this is achieved by specifying trainIdx and queryIdx.
    if(goodMatches.size() > minMatches){
        for(int i = 0; i < goodMatches.size(); i++){
            //get the keypoints from the good matches
            imgPts.push_back(keyPoints[goodMatches[i].trainIdx].pt);
            cameraPts.push_back(cameraKeyPoints[goodMatches[i].queryIdx].pt);
        }
        //once we have more than 5 of each,
        if(cameraPts.size() >5 && imgPts.size() > 5){
            //we use findHomography function provided by openCV using RANSAC algorithm
            homography = findHomography(imgPts,cameraPts,CV_RANSAC);
            //also once we find homography, we use another function provided my openCV, which
            //transforms the homography and outputs transformed boundaries around the detected image/object.
            perspectiveTransform(boundaries,imgBoundariesTransformed,homography);
        }
    }
}

//-----------------------------------------------------
//debug only
void OrbTracker::draw(){
    //drawing in this class was mostly used for DEBUGING
    ofPushStyle();
    //draw homography
    if(bDrawHomography){
        drawHomoGraphy();
    }
    //draw image feature descriptors
    if(bDrawDescriptors) {
        drawDescriptors();
    }
    //draw image features
    if(bDrawFeatures){
        drawFeatures();
    }
    ofPopStyle();
    
}

//-----------------------------------------------------
//debug only
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
void OrbTracker::drawFeatures(){
    //simply draw features as small green crosses
    //detected in the image, in real time
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
//debug only
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
void OrbTracker::drawDescriptors(){
    //same as features, we can draw descriptors detected.
    //we draw small circles around each of descriptors detected by openCV algorithms
    ofNoFill();
    ofSetColor(0, 255, 0);
    for(int i = 0; i < cameraKeyPoints.size(); i++){
        KeyPoint kp = cameraKeyPoints[i];
        float l = kp.response/1000;
        ofDrawCircle(kp.pt.x, kp.pt.y,  l);
        //we can also draw a line which shows the angle of the descriptor
        ofDrawLine(kp.pt.x, kp.pt.y, kp.pt.x + l*cos(kp.angle), kp.pt.y + l*sin(kp.angle));
    }
}

//-----------------------------------------------------
//debug only
//adapted from ofxSurfTracker addon for ofx and openCV for C examples.
void OrbTracker::drawHomoGraphy(){
    //draw the transformed bounding box
    ofNoFill();
    ofBeginShape();
    ofSetLineWidth(5);
    
    int cntrX;
    int cntrY;
    int scale;
    
    //imgBoundariesTransformed = 4 points
    //we take boudaries points and loop throught them
    for(int i = 0; i < imgBoundariesTransformed.size(); i++){
        //connect each of the points with a vertex point
        ofVertex(imgBoundariesTransformed[i].x, imgBoundariesTransformed[i].y);
        //draw a number on each of the point
        ofDrawBitmapString(i, imgBoundariesTransformed[i].x, imgBoundariesTransformed[i].y);
        
        //this is how we calculate the center X and Y coordinates of the box. Aswell as the scale for the animations.
        cntrX = (imgBoundariesTransformed[0].x+imgBoundariesTransformed[2].x)/2;
        cntrY = (imgBoundariesTransformed[0].y+imgBoundariesTransformed[2].y)/2;
        scale = ofDist(imgBoundariesTransformed[0].x, imgBoundariesTransformed[0].y, imgBoundariesTransformed[2].x, imgBoundariesTransformed[2].y);
        
    }
    
    //draw circles on each of the 4 points
    if(imgBoundariesTransformed.size() > 0){
        ofVertex(imgBoundariesTransformed[0].x, imgBoundariesTransformed[0].y);
        //draw sphere
        ofDrawSphere(cntrX,cntrY,200,scale/10);
    }
    ofEndShape();
}

//-----------------------------------------------------
//simple reset function to clear everything
void OrbTracker::reset(){
    goodMatchesAsKeyPoints.clear();
    imgBoundariesTransformed.clear();
    goodMatches.clear();
}

//-----------------------------------------------------
//   Get functions to retrieve required variables
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

//-----------------------------------------------------
//we use this function to return album image, so that it could be displayed in the gui.
ofImage * OrbTracker::getAlbumImage(int imgNo){
    if(imgNo==1){
        return &markerImg1;
    }else if(imgNo==2){
        return &markerImg2;
    }else if(imgNo==3){
        return &markerImg3;
    }
}
//-----------------------------------------------------
