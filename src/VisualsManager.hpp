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



class VisualsManager {
public:
    //Setup functions
    void setup();
    void setupManyBoxes();
    void setupIcoSphere();
    void setupCone();
    void setupSmallCubes();
    void setupSphereAndCones();
    void setupCylinders();
    void setupBoxesV3();


    //Update functions
    void update();
    void updateBoxesV3();

    
    //Drawing functions
    void drawManyBoxes(int posX, int posY, float param1, float param2, float param3);
    void drawIcoSphere(int posX, int posY, int posZ, float param1, float param2);
    void drawCone(int posX, int posY, int posZ, float param1, float param2);
    void drawSmallCubes(int posX, int posY, int posZ, float param1, float param2);
    void drawSphereAndCones(int posX, int posY, int posZ, float param1, float param2);
    void drawCylinders(int posX, int posY, int posZ, float param1, float param2);
    void drawBoxesV3(int posX, int posY, int posZ, float param1, float param2);

    
    //Helper functions
    
    //---smallCubes
    void positionAndDrawCubes(ofBoxPrimitive bigBox, vector <ofBoxPrimitive> &smallBoxes);

    
private:
    
    //Variables and objects
    
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
