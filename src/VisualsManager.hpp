//
//  VisualsManager.hpp
//  VinylAR2
//
//  Created by Tomas on 05/04/2017.
//
//

#ifndef VisualsManager_hpp
#define VisualsManager_hpp

#include "ofMain.h"
#include <stdio.h>
#include "ofxiOSPostGlitch.hpp"


class VisualsManager {
public:
    //Setup functions
    void setup();
    void setupShaders();
    void setupManyBoxes();
    void setupIcoSphere();
    void setupCone();
    void setupSmallCubes();
    void setupSphereAndCones();
    void setupCylinders();
    void setupBoxesV3();


    //Update functions
    void update(bool isPlaying, int posX, int posY, int posZ, int scale);
    void updateBoxesV3();

    
    //Drawing functions
    void draw(int posX, int posY, int posZ, int scale);
    void drawFbo();
    void drawManyBoxes(float param1, float param2, float param3);
    void drawIcoSphere(float param1, float param2);
    void drawCone(float param1, float param2);
    void drawSmallCubes(float param1, float param2);
    void drawSphereAndCones(float param1, float param2);
    void drawCylinders(float param1, float param2);
    void drawBoxesV3(float param1, float param2);

    
    //Helper functions
    
    //---smallCubes
    void positionAndDrawCubes(ofBoxPrimitive bigBox, vector <ofBoxPrimitive> &smallBoxes);

    //Set functions
    void setRMS(float rms);
    void setSpecFlatness(float sf);
    void setSpecCentroid(float sc);
    void setPeakFreq(float pf);
    void setFFTmagnitudes(float fftm);
    void setMFFCs(float mffcs);
    void setOctaveAverages(float oa);
    void setPitchHistogram(float ph);
    void setMelBands(float mb);
    void setIsBeat(bool beat);

    
private:
    
    //Variables and objects
    //Features to be set by MusicManager class
    float RMS;
    float specFlatness;
    float specCentroid;
    float peakFreq;
    float FFTmags;
    double MFFCs;
    float octaveAverages;
    float pitchHistogram;
    float melBands;
    bool isBeat;
    
    //Position variables
    float globalPosX;
    float globalPosY;
    float globalPosZ;
    float globalScale;
    
    //--ofxiOSPostGlitch shader variables
    ofxiOSPostGlitch effectShaders;
    int effectNumber;
    ofFbo myFbo;
    
    
    //---manyBoxesViz variables
    ofBoxPrimitive box;
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    vector <ofBoxPrimitive> boxes;
    vector <ofVboMesh> boxesSides[ofBoxPrimitive::SIDES_TOTAL];
    //---
    //---icoSphereViz variables
    ofIcoSpherePrimitive icoSphere;
    ofMaterial material;
    vector<ofMeshFace> triangles;
    int icoSphereMode;
    //---
    //---coneViz variables
    ofConePrimitive cone;
    ofVboMesh topCap, bottomCap, body;
    int coneMode;
    //---
    //---smallCubesViz variables
    ofBoxPrimitive cube;
    vector <ofBoxPrimitive> smallCubes;
    float smallCubesScale;
    int smallCubesSize;
    //---
    //---sphereAndConesViz variables
    ofSpherePrimitive sphere;
    vector <ofConePrimitive> cones;
    //---
    //---cylindersViz variables
    vector <ofCylinderPrimitive> cylinders;
    vector <ofVboMesh> tops;
    vector <ofVboMesh> bottoms;
    //---
    //---boxesV3 variables
    ofBoxPrimitive boxV3;
    ofVboMesh boxSidesV3[ofBoxPrimitive::SIDES_TOTAL];
    vector <ofBoxPrimitive> boxesV3;
    vector <ofVboMesh> boxesSidesV3[ofBoxPrimitive::SIDES_TOTAL];
    //---
    


    
    
};

#endif /* VisualsManager_hpp */
