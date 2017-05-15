//
//  VisualsManager.cpp
//  VinylARt
//
//  Created by Tomas on 05/04/2017.
//  This class deals with augmented reality visuals
//

#include "VisualsManager.hpp"
#include "ofxiOSPostGlitch.hpp"


//-----------------------------------------------------
//Setup function contains other setup functions which initialised all of thegraphics
void VisualsManager::setup(){
    
    setupShaders();
    setupIcoSphere();
    setupSmallCubes();
    setupSphereAndCones();
    setupCylinders();
    setupBoxesV3();
    setupOther();
    
}

//----------------------------------------------------
//Setup shaders
void VisualsManager::setupShaders(){
    //glitch
    myFbo.allocate(ofGetScreenWidth()+80, ofGetScreenHeight()+36);
    effectShaders.setupFBO(&myFbo);
    effectNumber = 0;
    
    //shader which is manipulated by sound
    shaderr.load("shaders/clrs");
}

//-----------------------------------------------------
//Function to set up other variables which did not fit into special setups
void VisualsManager::setupOther(){
    //sphr animation
    sphr.set(100, 5);
    
    //set initial radius and height for cone animation
    cone.set(50, 50);
    //set mode
    coneMode = 0;
    
    //sphere resolutions
    ofSetSphereResolution(20); //24
    
}

//-----------------------------------------------------
//Setup animation "IconSphere"
void VisualsManager::setupIcoSphere(){
    
    //set radius
    icoSphere.setRadius(10);
    //set material
    material.setShininess(120);
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    //set resolution of the sphere
    ofSetIcoSphereResolution(1); //24
    //initialise mode
    icoSphereMode = 0;
    //and create a copy of the first sphere to another one
    icoSphere2 = icoSphere;
    
}


//-----------------------------------------------------
//Setup animation "SmallCubes"
void VisualsManager::setupSmallCubes(){
    //size of cubes
    smallCubesSize = 70;
    //set the size
    cube.set(smallCubesSize);
    //create 6 small cubes and store them in the vector
    for(int i =0; i < 6; i ++){
        smallCubes.push_back(ofBoxPrimitive());
    }
    //for each of the small cubes we set the size and parent node which is a bigger 'cube'
    for(int i =0; i < smallCubes.size(); i++){
        smallCubes[i].setParent(cube);
        smallCubes[i].set(smallCubesSize/3);
    }
}

//-----------------------------------------------------
//Setup animation "SphereAndCones"
void VisualsManager::setupSphereAndCones(){
    //set initial sphere radius and heigh
    sphere.set(100,20);
    //set sphere resolution
    sphere.setResolution(10);
    
    //populate cones and cones2 vector with cones (7x)
    for(int i=0;i<7;i++){
        cones.push_back(ofConePrimitive());
        cones2.push_back(ofConePrimitive());
    }
    
    //for cones and cones2 vectors of cones set parent nodes, size and rotation
    for(int i=0;i<cones.size();i++){
        cones[i].set(36, 100);
        cones[i].setParent(sphere);
        cones2[i].set(36, 100);
        cones2[i].rotate(180, 180, 1, 1);
        cones2[i].setParent(sphere);
    }
}

//-----------------------------------------------------
//Setup animation "Cylinders"
void VisualsManager::setupCylinders(){
    //populate cylinders vector with 36 ofCylinderPrimitive shapes
    for(int i = 0; i < 36; i++){
        cylinders.push_back(ofCylinderPrimitive());
    }
}

//-----------------------------------------------------
//Setup animation "BoxesV3"
void VisualsManager::setupBoxesV3(){
    //set size of a box
    boxV3.set(10);
    
    //populate boxesV3 vector with box primitives and set their resolution
    for(int i = 0; i < 5; i++){
        boxesV3.push_back(ofBoxPrimitive());
        boxesV3[i].setResolution(1);
        
    }
    
    //get all of the side meshes(6) and store them in boxSidesV3 ofFbomesh.
    for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++){
        boxSidesV3[i] = boxV3.getSideMesh(i);
    }
    //now for every box in boxesV3 we get the sides and store them inside the boxesSidesV3 fboMesh vector
    for(int i = 0; i < boxesV3.size(); i++ ){
        for(int j = 0; j < ofBoxPrimitive::SIDES_TOTAL; j++){
            boxesSidesV3[i].push_back(boxesV3[i].getSideMesh(j));
        }
    }
    
}


//--- End of setup
//-----------------------------------------------------
//--- Update function
//-----------------------------------------------------
void VisualsManager::update(bool isPlaying, int posX, int posY, int posZ, float scale){
    isPlaying=true;
    //run times in update function so that we keep calculating the time
    switchTimer.update(1);
    shadersTimer.update(2);
    
    //start the fbo
    ///everything we put in here will be affected by the glitch effect shaders
    myFbo.begin();
    //clear everything even the alpha channel
    ofClear(0,0,0,0);
    
    //if music is playing (this comes from the arApp.mm) we start drawing
    if(isPlaying==true){
        //we set the global scale for animations
        globalScale = 0.5+scale;
        globalScale = ofClamp(globalScale,0,1.2); //limit the scale between 0 and 1.2 as sometimes this would get out of hand
        if(vizNum==8 &&!switchTimer.isFinished()){
            shaderr.begin();
            shaderr.setUniform1f("time",ofGetElapsedTimef());
            shaderr.setUniform2f("resolution",100.0,100.0);
            shaderr.setUniform2f("mouse",ofRandom(0.5,1),ofRandom(0.6,1));
        }
        
        //and this is where we draw all of the animations, using the position supplied
        ofPushMatrix();{
            //detailed explanation at draw() function
            draw(posX, posY, posZ);
        }ofPopMatrix();
        
        if(vizNum==8 &&!switchTimer.isFinished()){
            shaderr.end();
        }
    }
    
    myFbo.end();
    //end the fbo
    ////end shader effects
    
    //now we take the myFbo where we just drawn everything and pass it to shaders update function
    effectShaders.update(&myFbo);
    
}

//--- End of update
//-----------------------------------------------------
//--- Draw functions
//-----------------------------------------------------
// This is where I draw all of the animations. I have several different animations which are called
// in this method to produce the overall view.
void VisualsManager::draw(int posX, int posY, int posZ){
    //we set the global position to the positions retrieved from the update function
    globalPosX = posX;
    globalPosY = posY;
    globalPosZ = posZ;
    //set color to white
    ofSetColor(255);
    
    //here depending on the octave averages comming from MusicManager class
    //we choose which color scheme to use. All of this is done by manually setting threshold values
    if(specCentroid>0.1){
        
        float avg;
        for(int i=0; i < octaves->nAverages; i++) {
            avg += octaves->averages[i];
            
            clrScheme0=false;
            clrScheme1=false;
            clrScheme2=false;
            clrScheme3=false;
            clrScheme4=false;
            clrScheme5=false;
            
            //setting up threshold values for octave averages
            //we use these variables in some of the animations to determine colours
            if(avg<450){
                clrScheme0=true;
            }else if(avg>450 && avg<550){
                clrScheme1=true;
            }else if (avg>550 && avg<650){
                clrScheme2=true;
            }else if(avg>650 && avg<750){
                clrScheme3=true;
            }else if(avg>7500 && avg<850){
                clrScheme4=true;
            }else if(avg>850){
                clrScheme5=true;
            }
        }
        
    }
    ////glitch shaders from ofxiOSPostGlitch addon
    int shaderNo = ofRandom(0, 7);
    // 0 = colour split
    // 1 = soft shakyness, blurs out sides
    // 2 = shakes the screen slightly
    // 3 = chaotic wiggle, twists and turns
    // 4 = blurry squares, moving in blocks
    // 5 = produces slight wavey effect, does not distort
    // 6 = changes colours, seems like an inverse
    
    //if beat is detected we use random shader to 'spice up' the visuals
    if(isBeat){
        effectShaders.setShaderOn(shaderNo);
    }else{
        effectShaders.setShaderOff(shaderNo);
    }
    
    //here we use timer to randomly turn on the shader effects and trigger visualisations
    if(switchTimer.isFinished()){
        effectShaders.setShaderOn(0);
        effectShaders.setShaderOn(5);
        effectShaders.setShaderOn(2);
        vizNum = ofRandom(0,9);
    }
    //we turn them off here
    if(shadersTimer.isFinished()){
        effectShaders.setShaderOff(0);
        effectShaders.setShaderOff(5);
        effectShaders.setShaderOff(2);
    }
    
    
    if(specCentroid>0.001){
        
        //this huge switch statement controls which visuals to show.
        //all of the animation functions take in couple of parameters. These parameters modify
        //the sizes, rotations, colours and other things for different animations.
        switch(vizNum){
            case 0:
                ///0 cone with normals sticking out
                for(int i=0; i< 20; i++){
                    if(peakFreq>300){
                        drawCone(0,fft->magnitudes[i],specCentroid,RMS);
                    }else{
                        drawCone(1,100,specCentroid,RMS);
                    }
                }
                break;
                
            case 1:
                ///1 cube with small cubes on sides
                drawSmallCubes(RMS, peakFreq/50);
                break;
                
            case 2:
                ////2 many boxes turning into balls
                if(specCentroid>0.5){
                    drawManyBoxes(specCentroid,RMS,specFlatness,1);
                }else{
                    ofSetColor(255,255,255);
                    drawManyBoxes(specCentroid,peakFreq,300.0,0);
                }
                break;
                
            case 3:
                ////3 exploded IcoSphere
                if(specCentroid>0.4){
                    drawIcoSphere(0,RMS*100,peakFreq/100);
                }else{
                    drawIcoSphere(1,RMS*100,peakFreq/100);
                }
                break;
                
            case 4:
                ////4 circle + cones around
                if(specCentroid>0.001){
                    drawSphereAndCones(peakFreq/100, specCentroid, RMS);
                }
                break;
                
            case 5:
                ////5 cylinders
                if(specCentroid>0.01){
                    drawCylinders(RMS, specCentroid,isBeat);
                }
                break;
                
            case 6:
                ////6 splitup boxes
                if(specCentroid>0.01){
                    drawBoxesV3(RMS, specCentroid);
                    rndmNum = ofRandom(0, 2);
                }else if(isBeat){
                    rndmNum = ofRandom(0, 7);
                }else if(specFlatness>0.07){
                    rndmNum = ofRandom(0, 1);
                }
                break;
            case 7:
                ////7 spheres in a circle
                if(specCentroid>0.01){
                    drawSpheres(specCentroid);
                }
                break;
            case 8:
                ////8 spheres in one point
                if(specCentroid>0.01){
                    drawSphr(1);
                }else if(specCentroid>0.4){
                    drawSphr(2);
                }
                break;
        }
    }
    
}


//-----------------------------------------------------
//I have created the Fbo buffer so that we could draw shader effects on top of the animations
//the reason behind this is because I did not want shaders to affect video feed nor GUI.
void VisualsManager::drawFbo(){
    effectShaders.draw(0,0);
}

//-----------------------------------------------------
//Draw function for animation called "ManyBoxes"
void VisualsManager::drawManyBoxes(float param1, float param2, float param3, int boxStyle){
    ofSetSphereResolution(10);
    
    //boxStyle= 1 box, 0 sphere
    
    //create variables which control this animation using
    //parameters passed from audio analysis
    float cloudSize = ofGetWidth()/3;
    float maxBoxSize = ofClamp(param2*100,0,50);
    float spacing = ofClamp(param2*100,0,10);
    int boxCount = ofClamp(param3*120,0,50);
    //translate animation to the center position
    ofTranslate(globalPosX,globalPosY);
    //for each box/square we choose position based on signedNoise
    for(int i = 0; i < boxCount; i++) {
        ofPushMatrix();
        //we create a variable which imitated time named t
        float t = (param1 + i * spacing);
        //then we create a vector of x and y points which contains noise
        ofVec2f pos(ofSignedNoise(t, 0, 0),ofSignedNoise(0, t, 0));
        //boxsize is an area in which boxes/spheres are contained
        float boxSize = (globalScale*maxBoxSize) * ofSignedNoise(pos.x, pos.y, 0);
        //animate
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x);
        ofRotateY(pos.y);
        
        //change colours
        if(clrScheme0){
            ofSetColor(0,50+fft->magnitudes[i]*20,50);
        }else if(clrScheme1){
            ofSetColor(200+i,fft->magnitudes[i]*20,0);
        }else if(clrScheme2){
            ofSetColor(fft->magnitudes[i]*20,205+i,0);
        }else if(clrScheme3){
            ofSetColor(0,fft->magnitudes[i]*20,205+i);
        }
        
        //if style is 0 we draw spheres
        if(boxStyle==0){
            ofNoFill();
            ofDrawSphere(boxSize * 1.1f);
        }else if(boxStyle==1){
            //if style is 1 we draw boxes
            ofFill();
            ofDrawBox(boxSize * 1.1f);
        }
        ofPopMatrix();
    }
    
}

//-----------------------------------------------------
//Draw function for animation called "IcoSphere"
void VisualsManager::drawIcoSphere(int icoSphereMode,float param1, float param2){
    //set potition for spheres
    icoSphere.setPosition(globalPosX, globalPosY,globalPosZ);
    icoSphere2.setPosition(globalPosX, globalPosY,globalPosZ);
    //set rotation
    //param1=RMS*100
    icoSphere.rotate(abs(param1/100), 1.0, 0.0, 0.0);
    
    if(icoSphereMode==0){
        //this mode we draw mesh faces
        ofFill();
        //first we get triangles which make up the sphere
        triangles = icoSphere.getMesh().getUniqueFaces();
        ofVec3f normals;
        //then for each of the triangle we do the following:
        for(int i = 0; i < triangles.size(); i++ ) {
            //create new movement variable, which is made out of noise signal controlled by fft magnitudes
            float mvmnt = ofSignedNoise(fft->magnitudes[i] * i * 0.2, fft->magnitudes[i] * 0.02);
            //we also get facenormals, which are later on used to recreate circle shape out of triangles
            normals = triangles[i].getFaceNormal();
            for(int j = 0; j < 3; j++ ) {
                //we color triangles
                ofSetColor(ofRandom(100,130), ofRandom(100,255), ofRandom(100,110));
                //and set the vertex positions of new triangles using variables created before
                triangles[i].setVertex(j, triangles[i].getVertex(j) + normals * mvmnt );
            }
        }
        //we use triangle faces,vertices and normals to create a new sphere
        icoSphere.getMesh().setFromTriangles(triangles);
        //then draw that sphere
        icoSphere.drawFaces();
        
    }else if(icoSphereMode==1){
        //in this mode we draw wireframe and completed circle without exploded triangles
        ofSetColor(ofRandom(100,255),ofRandom(100,120), ofRandom(100,110));
        if(param1>0.2){
            icoSphere2.setScale(ofClamp(param1,1,10)*globalScale);
            icoSphere2.drawWireframe();
        }else{
            icoSphere2.setScale(param2*globalScale);
            icoSphere2.draw();
        }
        //we set the original sphere to become equal to icosphere2 in order to restore faces and vertices
        icoSphere = icoSphere2;
    }
    
    
}
//-----------------------------------------------------
//Draw function for animation called "Spheres"
void VisualsManager::drawSpheres(float param1){
    //begin spheres shape
    ofBeginShape();
    ofFill();
    //we want to draw 14 spheres
    for(int i=0; i < 14; i++) {
        //select color for them
        if(param1>0.04){
            ofSetColor(fft->magnitudes[i]*i*20,133,133);
        }else if(param1>0.1){
            ofSetColor(7*i,fft->magnitudes[i]*i*20,133);
        }else{
            ofSetColor(20*i,133,fft->magnitudes[i]*i*20);
        }
        //place spheres in a circle using trig functions
        int x = cos(i)*100;
        int y = sin(i)*100;
        //and draw them affecting the size by the fft
        ofDrawSphere(globalPosX+x,globalPosY+y,150,5+fft->magnitudes[i]*2);
    }
    ofEndShape();
}

//-----------------------------------------------------
//Draw function for animation called "Sphr"
void VisualsManager::drawSphr(float param1){
    //12 spheres, set position, size and draw wireframes
    for(int i=12; i> 0; i--){
        sphr.setPosition(globalPosZ+150, globalPosY,0);
        sphr.set(fft->magnitudes[i]*2, i*2);
        if(param1==0){
            sphr.draw();
        }else{
            sphr.drawWireframe();
            
        }
    }
    
}

//-----------------------------------------------------
//Draw function for animation called "Cone"
void VisualsManager::drawCone(int coneMode, float param1, float param2, float param3){
    
    //we set the position for the cone animation
    cone.setPosition(globalPosX, globalPosY, globalPosZ);
    //take angle and set it to the param2. It tells us the speed at which the cone will rotate
    float angle = param2*2;
    //rotate animation depending on param2
    cone.rotate(angle, ofRandom(0.1,1.0), 0, 0);
    //play around with colours
    //if codemode is 0 we set the colour to white
    if(coneMode==0){
        ofSetColor(255);
    }else if(coneMode==1){
        //if cone mode is 1 we set the cone to different colours
        //while using audio parameters to control the richness of the colours
        
        if(clrScheme0){ //variation of red
            ofSetColor(250,0,100+param2*100);
            
        }else if(clrScheme1){//variation of orange
            ofSetColor(255-(mfcc->melBands[(int)ofRandom(0,42)]*4),69+mfcc->melBands[(int)ofRandom(0,42)],100-param2*100);
            
        }else if(clrScheme2){//variation of yellow
            ofSetColor(255+mfcc->melBands[(int)ofRandom(0,42)],255+mfcc->melBands[(int)ofRandom(0,42)],100+param2*100);
            
        }else if(clrScheme3){//variation of green
            ofSetColor(255+mfcc->melBands[(int)ofRandom(0,42)],255+mfcc->melBands[(int)ofRandom(0,42)],100-param2*100);
            
        }else if (clrScheme4){//variation of blue
            ofSetColor(75,param2*100,255+mfcc->melBands[(int)ofRandom(0,42)]);
            
        }else if(clrScheme5){//variation of indigo
            ofSetColor(75,param2*100,130+mfcc->melBands[(int)ofRandom(0,42)]);
            
        }
    }
    //in the end we draw the final shape.
    //we draw the normals which are sticking out of the shape
    cone.drawNormals(param1, false);
    //and we draw the wireframe
    cone.setScale(globalScale+0.5);
    cone.drawWireframe();
    //set scale of the shape
    cone.setScale(globalScale);
    //and then finally full cone
    cone.draw();
}

//-----------------------------------------------------
//Draw function for animation called "SmallCubes"
void VisualsManager::drawSmallCubes(float param1, float param2){
    //param1=rms
    //param2=peakfreq/50
    
    //position cubes and set the scale of the animtion
    cube.setPosition(globalPosX,globalPosY,globalPosZ);
    cube.setScale(globalScale);
    
    
    
    //play around with the colours using audio features passed as parameters
    if(param1<0.02){
        ofSetColor(0,100,100);
    }else if(param1>0.02){
        ofSetColor(ofRandom(0,255),ofRandom(0,255),100);
    }else if(param1>0.2){
        ofSetColor(ofClamp(param2,0,255),0,100);
    }
    
    //draw the big cube
    cube.draw();
    
    //set rotation to be equal to RMS*100
    float angle = param1 * 100;
    
    //rotate cube
    cube.rotate(angle, angle, 0, angle/10);
    //set size of the cub depending on the peakFreq
    cube.set(ofClamp(param2*5, 20, 100));
    
    //drawing biggest cube
    if(param1 > 0.1){
        ofSetColor(50, 255, 50);
        cube.drawWireframe();
    }
    
    //drawing small cubes on the sides
    positionAndDrawCubes(cube,smallCubes);
    
}

//-----------------------------------------------------
//Draw function for animation called "SphereAndCones"
void VisualsManager::drawSphereAndCones(float param1, float param2,float param3){
    //set line width of the middle sphere
    ofSetLineWidth(5);
    
    //set position, scale, resolution and rotation of the middle sphere
    sphere.setPosition(globalPosX, globalPosY,globalPosZ);
    sphere.setResolution(param3*50);
    sphere.setScale(globalScale);
    sphere.rotate(param1,0,param1,0);
    
    //color of the mid sphere
    if(isBeat){
        ofSetColor(ofRandom(250,120), ofRandom(50,110),ofRandom(100,120));
    }else{
        ofSetColor(255);
    }
    //draw middle sphere wireframe
    sphere.drawWireframe();
    
    //we loop throught cones vector and do the following:
    for(int i=0;i<cones.size();i++){
        //define x and z postions of each of the cones
        int x = cos(i)*110;
        int z = sin(i)*110;
        
        //set their scales(sizes) these are affected by fft magnitudes
        cones[i].setScale(globalScale*fft->magnitudes[5+i]/8);
        cones2[i].setScale(globalScale*fft->magnitudes[5+i]/8);
        
        //use the x and z positions defined before to position cones around
        if(param2>0.7){
            cones[i].setPosition(x, 0, z);
            cones2[i].setPosition(x,0, z);
        }else{
            //position them in a different position
            cones[i].setPosition(x, -50, z);
            cones2[i].setPosition(x, 50, z);
        }
        
        if(isBeat){
            ofSetColor(255);
        }else{
            //lastly apply different colours
            if(param3>0.02){
                ofSetColor(ofRandom(250,120), ofRandom(50,110),ofRandom(100,120));
            }else if(param3>0.04){
                ofSetColor(ofRandom(100,120),ofRandom(100,120),ofRandom(150,110));
            }else if(param3>0.08){
                ofSetColor(ofRandom(255,180), ofRandom(50,110),ofRandom(100,120));
            }
        }
        //and draw cones
        cones[i].draw();
        cones2[i].draw();
    }
}

//-----------------------------------------------------
//Draw function for animation called "Cylinders"
void VisualsManager::drawCylinders(float param1, float param2, bool param3){
    
    //positioning cylinders by looping throught the vector populated in setup
    for(int i = 0; i < cylinders.size(); i++){
        //set x and y positions
        int x = cos(i)*100;
        int y = sin(i)*100;
        //set initial sie, scale ,and positions
        cylinders[i].set(20, 50);
        cylinders[i].setScale(globalScale);
        cylinders[i].setPosition(globalPosX+x, globalPosY+y, globalPosZ+fft->magnitudes[i]*10);
        //here we split cilinders. We take tops parts and bottom parts of the cilynders
        //and store them into specific vectors
        tops.push_back(cylinders[i].getTopCapMesh());
        bottoms.push_back(cylinders[i].getBottomCapMesh());
    }
    
    //yet again, colour selection
    if(clrScheme0){ //red
        ofSetColor(250,fft->magnitudes[(int)ofRandom(0,42)] * 2,50);
    }else if(clrScheme1){//orange
        ofSetColor(250,50,fft->magnitudes[(int)ofRandom(0,42)] * 2);
    }else if(clrScheme2){//yellow
        ofSetColor(250,50,50);
    }else if(clrScheme3){//green
        ofSetColor(fft->magnitudes[(int)ofRandom(0,42)] * 2,50,50);
    }else if (clrScheme4){//blue
        ofSetColor(50,fft->magnitudes[(int)ofRandom(0,42)] * 2,50);
    }else if(clrScheme5){//indigo
        ofSetColor(75+(int)fft->magnitudes[(int)ofRandom(0,42)] * 2,0,ofRandom(255));
    }
    
    ofPushMatrix();
    //we loop through cylinder vector and do the following
    for(int i = 0; i < cylinders.size(); i++){
        //enable cylinders to be tranfsormable
        cylinders[i].transformGL();
        //draw tops
        ofPushMatrix(); {
            if(tops[i].getNumNormals() > 0) {
                ofTranslate(tops[i].getNormal(0) * fft->magnitudes[i] * 2 );
                tops[i].draw();
            }
        } ofPopMatrix();
        
        //draw bottoms
        ofPushMatrix(); {
            if(bottoms[i].getNumNormals() > 0) {
                ofTranslate(bottoms[i].getNormal(0).x, bottoms[i].getNormal(0).y * (fft->magnitudes[i] * -2), bottoms[i].getNormal(0).z);
                bottoms[i].draw();
            }
        } ofPopMatrix();
        
        //restore transformation
        cylinders[i].restoreTransformGL();
        
        ofSetLineWidth(1);
        
        //lastly we draw wireframes when beat is detected
        if(param3){
            if(i%2 == 0){
                cylinders[i].drawWireframe();
            }
        }
    }
    ofPopMatrix();
}



//-----------------------------------------------------
//Draw function for animation called "BoxesV3"
void VisualsManager::drawBoxesV3(float param1, float param2){
    
    //for each of the boxes in the vector we set the postition and scale
    for(int i = 0; i < boxesV3.size(); i++){
        boxesV3[i].setPosition(globalPosX, globalPosY,globalPosZ);
        boxesV3[i].setScale(globalScale);
    }
    
    //we also transfrom the boxes, and slit their sides
    for(int i = 0; i < boxesV3.size(); i++){
        boxesV3[i].transformGL();
        // then for each side
        for(int j = 0; j < ofBoxPrimitive::SIDES_TOTAL; j++ ) {
            ofPushMatrix();
            //we choose colours using variables from sound class
            switch(rndmNum){
                case 0:
                    ofSetColor(0,100+(i+30),fft->magnitudes[i]*10);
                    break;
                case 1:
                    ofSetColor((param1*10)+(i*20),0,0);
                    break;
                case 2:
                    ofSetColor(0+(i*20),0,fft->magnitudes[i]*10);
                    break;
                case 3:
                    ofSetColor((param1*10)+(i*20),0,0);
                    break;
                case 4:
                    ofSetColor(fft->magnitudes[i]*10,0+(i*20),0);
                    break;
                case 5:
                    ofSetColor(0+(i*20),(param2*10),0);
                    break;
                case 6:
                    ofSetColor(0,fft->magnitudes[i]*10,0+(i*20));
                    break;
            }
            
            //transform them using fft magnitudes
            ofTranslate(boxesSidesV3[i][j].getNormal(0) * fft->magnitudes[i]*5);
            //and draw the boxes
            boxesSidesV3[i][j].drawFaces();
            ofPopMatrix();
        }
        //and finally restore boxes GL positions to original
        boxesV3[i].restoreTransformGL();
    }
}


//--- End of draw
//-----------------------------------------------------
//--- Helper functions

//-----------------------------------------------------
//Function which is needed to draw "smallCubes" animation. It is a separate function as it has a lot of code
//which deals with position and drawing of small cubes.
void VisualsManager::positionAndDrawCubes(ofBoxPrimitive bigBox, vector <ofBoxPrimitive> &smallBoxes){
    
    //varaitions of green
    ofSetColor(50,50,ofRandom(255));
    
    //if beat set cubes to be white
    if(isBeat){
        ofSetColor(255);
    }
    
    //we set the positions of each of the small cubes around the bigger box using this manual method
    smallBoxes[0].setPosition(0,0,(bigBox.getWidth()/2)+smallBoxes[0].getWidth()/2);
    smallBoxes[1].setPosition(0,0,(-bigBox.getWidth()/2)-smallBoxes[1].getWidth()/2);
    smallBoxes[2].setPosition(0,(bigBox.getWidth()/2)+smallBoxes[2].getWidth()/2,0);
    smallBoxes[3].setPosition(0,(-bigBox.getWidth()/2)-smallBoxes[3].getWidth()/2,0);
    smallBoxes[4].setPosition((bigBox.getWidth()/2)+smallBoxes[4].getWidth()/2,0,0);
    smallBoxes[5].setPosition((-bigBox.getWidth()/2)-smallBoxes[5].getWidth()/2,0,0);
    
    //lastly we set the scale of smallboxes and draw them to the screen
    for(int i =0; i < smallBoxes.size(); i++){
        smallBoxes[i].setScale(globalScale);
        if(isBeat){
            //is beat draw wireframe instead of actual cube
            smallBoxes[i].drawWireframe();
        }else{
            //finally draw the cubes
            smallBoxes[i].draw();
        }
    }
}

//--- Set Functions
// they are used to set variables in this class coming in from MusicManager
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
void VisualsManager::setIsBeat(bool beat){
    isBeat = beat;
}

//-----------------------------------------------------
void VisualsManager::setFFT(ofxMaxiFFT &mfft){
    fft = &mfft;
}

//-----------------------------------------------------
void VisualsManager::setMFCC(maxiMFCC &myMfcc){
    mfcc = &myMfcc;
}

//-----------------------------------------------------
void VisualsManager::setFFTOcatveAnalyzer(ofxMaxiFFTOctaveAnalyzer &myOctave){
    octaves = &myOctave;
    
}



