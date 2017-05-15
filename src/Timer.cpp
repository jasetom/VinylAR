//
//  Timer.cpp
//  VinylARt
//
//  Created by Tomas on 18/04/2017.
//
//

#include "Timer.hpp"

//This is a simple timer class used to switch between shaders and animations

//-----------------------------------------------------
// Update function to count the time
void Timer::update(int type){
    
    //type 1 is for animations - random from 3 seconds to 7 seconds
    //type 2 is for shaders - 1 second
    
    //if timer is finished
    if (bTimerFinished) {
        //start timer and set it to false
        bTimerFinished = false;
        //start time is current ellapsed time
        timerStartTime = ofGetElapsedTimeMillis();
        //timer for animations
        if(type==1){
            //3 to 7 seconds
            timerEndTime = ofRandom(3000, 7000);
        }
    }
   
    //create new timer varaible every time, and take away ellapsed time from startime
    int timer = ofGetElapsedTimeMillis() - timerStartTime;
    //when the equation is less then end time and timer is not finished,
    if(timer >= timerEndTime && !bTimerFinished) {
        //end the timer
        bTimerFinished = true;
        if(type==2){
            //and set timer end time to 1second if we use this timer for shaders
            timerEndTime = 1000;
        }
    }
}

//-----------------------------------------------------
// Check if timer is running or finished
bool Timer::isFinished(){
    return bTimerFinished;
}
