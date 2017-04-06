//
//  VisualsManager.cpp
//  VinylAR2
//
//  Created by Tomas on 05/04/2017.
//
//

#include "VisualsManager.hpp"

//-----------------------------------------------------
void VisualsManager::setup(){
    
    setupManyBoxes();
    setupIcoSphere();
    setupCone();
    setupSmallCubes();
    setupSphereAndCones();
    setupCylinders();
    
}

//-----------------------------------------------------
void VisualsManager::setupManyBoxes(){
    
    ofSetLineWidth(10);
//    ofSetBackgroundAuto(false);

}

//-----------------------------------------------------
void VisualsManager::setupIcoSphere(){
    
    icoSphere.setRadius( 10 );
    
    material.setShininess(120);
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    ofSetSphereResolution(24);
    icoSphereMode = 0;
    
}

//-----------------------------------------------------
void VisualsManager::setupCone(){
    
    cone.set(50, 50);
    
    coneMode = 0;
    
}

//-----------------------------------------------------
void VisualsManager::setupSmallCubes(){
    smallCubesSize = 70;
    
    cube.set(smallCubesSize);
    
    for(int i =0; i < 6; i ++){
        smallCubes.push_back(ofBoxPrimitive());
    }
    
    for(int i =0; i < smallCubes.size(); i++){
        smallCubes[i].setParent(cube);
        smallCubes[i].set(smallCubesSize/3);
    }
}

//-----------------------------------------------------
void VisualsManager::setupSphereAndCones(){
    
    sphere.set(100,20);
    
    for(int i=0;i<10;i++){
        cones.push_back(ofConePrimitive());
    }
    
    for(int i=0;i<cones.size();i++){
        cones[i].set(36, 100);
        cones[i].setParent(sphere);
        cones[i].lookAt(sphere);
    }
}

//-----------------------------------------------------
void VisualsManager::setupCylinders(){

    for(int i = 0; i < 36; i++){
        cylinders.push_back(ofCylinderPrimitive());
    }
    
}

//-----------------------------------------------------
void VisualsManager::setupBoxesV3(){
    
    boxV3.set(50);
    
    //boxes
    for(int i = 0; i < 20; i++){
        boxesV3.push_back(ofBoxPrimitive());
        boxesV3[i].setResolution(1);
        
    }

}



//--- End of setup
//-----------------------------------------------------
//--- Update functions

//-----------------------------------------------------
void VisualsManager::update(){
    
    updateBoxesV3();
}

//-----------------------------------------------------
void VisualsManager::updateBoxesV3(){
    // store the box sides so that we can manipulate them later //
    for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++){
        boxSidesV3[i] = boxV3.getSideMesh(i);
    }
    
    for(int i = 0; i < boxesV3.size(); i++ ){
        for(int j = 0; j < ofBoxPrimitive::SIDES_TOTAL; j++){
            boxesSidesV3[i].push_back(boxesV3[i].getSideMesh(j));
        }
    }
}



//--- End of update
//-----------------------------------------------------
//--- Draw functions

//-----------------------------------------------------
void VisualsManager::draw(int posX, int posY, int posZ, int scale){

    
}


//-----------------------------------------------------
void VisualsManager::drawManyBoxes(int posX, int posY, float param1, float param2, float param3){
    
//    cout << "visualsManager RMS: " << RMS <<endl;


    
    //boxesv2
    
//    float cloudSize = param1/2.0;
        float cloudSize = ofGetWidth()/2;
    
    float maxBoxSize = 50;
    //    float maxBoxSize = mouseY/100.0;
    
    float spacing = 1;
    int boxCount = 100;
    
    ofTranslate(posX,posY);
//    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    for(int i = 0; i < boxCount; i++) {
        ofPushMatrix();
        
        float t = (ofGetElapsedTimef() + i * spacing);
        ofVec3f pos(
                    ofSignedNoise(t, 0, 0),
                    ofSignedNoise(0, t, 0),
                    ofSignedNoise(0, 0, t));
        
                float boxSize = maxBoxSize * ofSignedNoise(pos.x, pos.y, pos.z);
//        float boxSize = param2/10 * ofSignedNoise(pos.x, pos.y*param2/100.0, pos.z);
        
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x);
        ofRotateY(pos.y);
        ofRotateZ(pos.z);
        
        ofNoFill();
        ofSetColor(ofColor::fromHsb(sinf(param2/100.0) * 128 + 128, 255, 255),50);
        //        ofDrawSphere(boxSize * 1.1f);
        ofDrawBox(boxSize * 1.1f);
        
        ofPopMatrix();
    }

    
    
}

//-----------------------------------------------------
void VisualsManager::drawIcoSphere(int posX, int posY, int posZ, float param1,float param2){
    
    icoSphereMode = 1;
    ofSetColor(100);
    
    icoSphere.setPosition(posX, posY,posZ);
    
    icoSphere.rotate(abs(param1/100), 1.0, 0.0, 0.0);
    
    ofSetColor(0,255,0);
    
    
    if(icoSphereMode ==1){
        
        ofFill();
        
        triangles = icoSphere.getMesh().getUniqueFaces();
        
//        float angle = (ofGetElapsedTimef() * 1.4);
        ofVec3f faceNormal;
        for(size_t i = 0; i < triangles.size(); i++ ) {
            float frc = ofSignedNoise(param2/100* (float)i * .1, 11*.05) * 4;
            faceNormal = triangles[i].getFaceNormal();
            for(int j = 0; j < 3; j++ ) {
                triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc );
            }
        }
        icoSphere.getMesh().setFromTriangles( triangles );
    }
    
    icoSphere.drawFaces();
    
}

//-----------------------------------------------------

void VisualsManager::drawCone(int posX, int posY, int posZ, float param1, float param2){
   
    coneMode =0;
    cone.setPosition(posX, posY, posZ);

    ofSetColor(255,0,0);
    
    float angle = (ofGetElapsedTimef() * 1.4);

    
    cone.rotate(angle/100, 1.0, 0.0, 0.0);
    
    if(coneMode == 1) {
        bottomCap   = cone.getCapMesh();
        body        = cone.getConeMesh();
    }
    
    cone.drawNormals(param1/10, false);
    cone.setScale(1.01f);
    
    cone.drawWireframe();
    cone.setScale(.9f);
    cone.draw();
    
    
}

//-----------------------------------------------------
void VisualsManager::drawSmallCubes(int posX, int posY, int posZ, float param1, float param2){

    cube.setPosition(posX,posY,posZ);

    ofSetColor(144);
    cube.draw();
    ofSetColor(0);
    
    float angle = (ofGetElapsedTimef() * 1.4);

    
    cube.rotate(angle/100, angle/100, 0, 0);

    //drawing biggest cube
    cube.drawWireframe();
    //drawing small cubes
    positionAndDrawCubes(cube,smallCubes);
    ofSetColor(255,0,0);

}

//-----------------------------------------------------
void VisualsManager::drawSphereAndCones(int posX, int posY, int posZ, float param1, float param2){

    ofSetColor(0);

    sphere.setPosition(posX, posY,posZ);

    sphere.drawWireframe();

    for(int i=0;i<cones.size();i++){
        int x = cos(i)*300;
        int z = sin(i)*300;
        cones[i].setPosition(x,-sphere.getRadius()/2,z);
        
        cones[i].draw();
    }
    
    sphere.rotate(param1,0,param1,0);

}

//-----------------------------------------------------
void VisualsManager::drawCylinders(int posX, int posY, int posZ, float param1, float param2){

    for(int i = 0; i < cylinders.size(); i++){
        int x = cos(i)*100;
        int z = sin(i)*100;
        
        cylinders[i].set(20, 50);
        cylinders[i].setPosition(posX+x, posY+z,posZ);
    }

    
    
    for(int i = 0; i < cylinders.size(); i++){
        tops.push_back(cylinders[i].getTopCapMesh());
    }
    
    for(int i = 0; i < cylinders.size(); i++){
        bottoms.push_back(cylinders[i].getBottomCapMesh());
    }
    
    
    //    topCap      = cylinder.getTopCapMesh();
    //    bottomCap   = cylinder.getBottomCapMesh();
    //    body        = cylinder.getCylinderMesh();
    
    
    
    
    
    for(int i = 0; i < cylinders.size(); i++){
        cylinders[i].transformGL();
        
        
        ofPushMatrix(); {
            
            ofSetColor(0,233,100);
            if(tops[i].getNumNormals() > 0) {
                ofTranslate( tops[i].getNormal(0) * param1*ofRandom(.1));
                tops[i].draw();
            }
            
            ofSetColor(233,0,100);
            if(bottoms[i].getNumNormals() > 0) {
                ofTranslate( bottoms[i].getNormal(0) * -param1*ofRandom(.2));
                bottoms[i].draw();
            }
            
            
        } ofPopMatrix();
        cylinders[i].rotate(param1, param1/100,  0,0);
        
        ofSetColor(255);
        cylinders[i].drawWireframe();
        
    }

    
    
}

    
    
//-----------------------------------------------------
void VisualsManager::drawBoxesV3(int posX, int posY, int posZ, float param1, float param2){
    
    
    // Box //
    //boxV3.setPosition(posX, posY, posZ);
    
    
    for(int i = 0; i < boxesV3.size(); i++){
        int x = cos(i)*10;
        int z = sin(i)*10;
        boxesV3[i].setPosition(posX+x, posY+z,posZ);
        
    }
    
 
    for(int i = 0; i < boxesV3.size(); i++){
        ofSetColor(0,233,i*10);
        boxesV3[i].transformGL();
        for(int j = 0; j < ofBoxPrimitive::SIDES_TOTAL; j++ ) {
            ofPushMatrix();
            ofTranslate(boxesSidesV3[i][j].getNormal(0) * sin(ofGetElapsedTimef()));
            
            //        ofTranslate( boxSides[i].getNormal(0) * mouseX  );
            //                   ofSetColor(ofFloatColor(1.0,0.0,0.2).lerp(ofFloatColor(0.0,4.0,0.2), i*ofGetElapsedTimef()));
            boxesSidesV3[i][j].drawFaces();
            ofPopMatrix();
        }
        boxesV3[i].restoreTransformGL();
        
        ofSetColor(233,100,0);
        boxesV3[i].drawWireframe();
        boxesV3[i].draw();
        
        //boxesV3[i].rotate(param2/100,param1/100,0,0);
        //            boxesSides
    }
    
    
    
    
    
    
    
    
}


//--- End of draw
//-----------------------------------------------------
//--- Helper functions

//-----------------------------------------------------
void VisualsManager::positionAndDrawCubes(ofBoxPrimitive bigBox, vector <ofBoxPrimitive> &smallBoxes){
    
    smallBoxes[0].setPosition(0,0,(bigBox.getWidth()/2)+smallBoxes[0].getWidth()/2);
    smallBoxes[1].setPosition(0,0,(-bigBox.getWidth()/2)-smallBoxes[1].getWidth()/2);
    smallBoxes[2].setPosition(0,(bigBox.getWidth()/2)+smallBoxes[2].getWidth()/2,0);
    smallBoxes[3].setPosition(0,(-bigBox.getWidth()/2)-smallBoxes[3].getWidth()/2,0);
    smallBoxes[4].setPosition((bigBox.getWidth()/2)+smallBoxes[4].getWidth()/2,0,0);
    smallBoxes[5].setPosition((-bigBox.getWidth()/2)-smallBoxes[5].getWidth()/2,0,0);
    
    for(int i =0; i < smallBoxes.size(); i++){
        smallBoxes[i].draw();
    }
    
}

//--- Setting variables coming in from audio class
//-----------------------------------------------------
void VisualsManager::setRMS(float rms){
   RMS = rms;
}

//-----------------------------------------------------
void VisualsManager::setSpecFlatness(float sf){
    specFlatness = sf;
}

//-----------------------------------------------------
void VisualsManager::setSpecCentroid(float sc){
    specCentroid = sc;
}

//-----------------------------------------------------
void VisualsManager::setPeakFreq(float pf){
    peakFreq = pf;
}

//-----------------------------------------------------
void VisualsManager::setFFTmagnitudes(float fftm){
    FFTmags = fftm;
}

//-----------------------------------------------------
void VisualsManager::setMFFCs(float mffcs){
    MFFCs = mffcs;
}

//-----------------------------------------------------
void VisualsManager::setOctaveAverages(float oa){
    octaveAverages = oa;
}

//-----------------------------------------------------
void VisualsManager::setPitchHistogram(float ph){
    pitchHistogram = ph;
}

//-----------------------------------------------------
void VisualsManager::setMelBands(float mb){
    melBands = mb;
};
